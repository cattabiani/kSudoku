#include "board.hpp"
#include "utils.hpp"

#include <iostream>
#include <unordered_set>
#include <queue>




Board::Board() : 
    b_(vector<size_t>(81, 0)), 
    flags_r_(vector<bitset<9>>(9, 0)), 
    flags_c_(vector<bitset<9>>(9, 0)), 
    flags_sq_(vector<bitset<9>>(9, 0)),
    rng_(std::mt19937(dev_())) {
}

bitset<9> Board::availability(const size_t r, const size_t c) const {

    auto ans = flags_r_[r] | flags_c_[c] | flags_sq_[sq(r, c)];

    ans.flip();
    return ans;  
}

size_t Board::count_availables(const size_t r, const size_t c) const {
    if ((*this)(r, c)) {
        return 0;
    }

    return availability(r, c).count();
}

size_t Board::count_availables(const size_t idx) const {
    const auto p = idx2rc(idx);
    return count_availables(p.first, p.second);
}

void Board::rand_set(const size_t r, const size_t c) {
    auto av = availability(r, c);
    auto n = count_availables(r, c);

    if (n <= 0) {
        throw std::logic_error("No availability!");
    }

    uniform_int_distribution<mt19937::result_type> dist(0,n-1);
    n = dist(rng_);
    auto idx = Utils::idx_nth_true_bit(av, n);

    auto res = set(r, c, idx+1);
    if (!res) {
        throw std::logic_error("I could not set the value");
    }
}

void Board::rand_set(const size_t idx) {
    const auto p = idx2rc(idx);
    return rand_set(p.first, p.second);
}

/* Interesting function

Here we assign to all the squares of the sudoku a ranking equal to the number of possible numbers they can have.
After, we stuff all the squares in a minheap. For 81 times we:
 - pull from the heap
 - We get a square with the least amount of possible choices. 
 - We assign a choice randomly. 
 - We update the ranking.
*/
void Board::rand() {
    using P = pair<size_t, size_t>;
    auto cmp = [](P a, P b) {return a.first > b.first; };


    priority_queue<P,vector<P>, decltype(cmp)> q(cmp);

    for (auto i = 0; i < 81; ++i) {
        q.push({9, i});
    }

    for (int iii = 0; iii < 81; ++iii) {

        const auto qp = q.top();
        q.pop();

        const auto idx = qp.second;

        // if we extract a square that we already assigned just skip. Lazy garbage collection
        if (!count_availables(idx)) {
            --iii;
            continue;
        }
        const auto p = idx2rc(idx);

        auto r = p.first;
        auto c = p.second;
        auto idx_sq = sq(r, c);

        rand_set(r, c);

        // updates. I could do this only for the unassigned squares but the 
        // complexity would remain the same and the additional math is prone to errors
        
        //update the column rankings
        for (auto i = 0; i < 9; ++i) {
            const auto cc = count_availables(r, i);
            if (cc) {
                q.push({cc, rc2idx(r, i)});
            }
        }

        //update the row rankings
        for (auto i = 0; i < 9; ++i) {
            const auto cc = count_availables(i, c);
            if (cc) {
                q.push({cc, rc2idx(i, c)});
            }
        }

        //update the square rankings
        {
            auto i = 0;
            for (auto m : (*this)[idx_sq]) {
                auto m_idx = (&m) - (&b_.front());
                const auto cc = count_availables(i, c);
                if (cc) {
                    q.push({cc, rc2idx(i, c)});
                }
                ++i;
            }
        }
    }

}

Board::SquareView Board::operator[](const size_t sq) {
    auto r = 3*(sq/3);
    auto c = 3*(sq%3);
    return {&b_[Board::rc2idx(r, c)]};
}


size_t& Board::operator()(const size_t r, const size_t c) {
    return b_[rc2idx(r, c)];
}

const size_t& Board::operator()(const size_t r, const size_t c) const {
    return b_[rc2idx(r, c)];
}


size_t Board::rc2idx(const size_t r, const size_t c) {
    return 9*r + c;
}

pair<size_t, size_t> Board::idx2rc(const size_t idx) {
    return {idx/9, idx%9};
}

size_t Board::sq(const size_t r, const size_t c) const {
    return 3*(r/3) + (c/3);
}

bool Board::is_available(const size_t r, const size_t c, const size_t v) const {
    return (!flags_c_[c].test(v)) && (!flags_r_[r].test(v)) && (!flags_sq_[sq(r, c)].test(v));
}

bool Board::set(const size_t r, const size_t c, const size_t v) {

    if (!is_available(r, c, v-1)) {
        return false;
    }

    flags_c_[c].set(v-1);
    flags_r_[r].set(v-1);
    flags_sq_[sq(r, c)].set(v-1);

    (*this)(r, c) = v;
    return true;
}

bool Board::set(const size_t idx, const size_t v) {
    const auto p = idx2rc(idx);
    return set(p.first, p.second);
}

ostream & operator << (ostream & o, const Board & b) {
    o << endl;
    for (auto i = 0; i < 9; ++i) {
        for (auto j = 0; j < 9; ++j) {
            o << b(i, j) << ' ';
            if ((j+1)%3 == 0 && j != 8) {
                o << "| ";
            }
        }
        o << endl;
        if ((i+1)%3 == 0 && i != 8) {
            o << "---------------------" << endl;
        }
    }
    return o;
}
