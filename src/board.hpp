#pragma once

#include <vector>
#include <bitset>
#include <ostream>
#include <random>
#include <iostream>

using namespace std;

class Board {
public:
    struct SquareView {
        using pointer           = size_t*;
        public:
            SquareView(pointer ptr) : m_ptr_(ptr) {}
        struct Iterator 
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = size_t;
            using pointer           = size_t*;
            using reference         = size_t&;

            Iterator(pointer ptr) : m_ptr_(ptr), c_(0) {}

            reference operator*() const { 

                
                return *(m_ptr_ + c_ ); }
            pointer operator->() { return m_ptr_ + c_; }
            Iterator& operator++() { 
                c_++;
                if (c_ == 3 || c_ == 12) c_ += 6;  
                return *this; 
            }  
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
            friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr_+a.c_ == b.m_ptr_+b.c_; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr_+a.c_ != b.m_ptr_+b.c_; };  

        private:
            pointer m_ptr_;
            size_t c_;
        };

        Iterator begin() { return Iterator(&m_ptr_[0]); }
        Iterator end()   { return Iterator(&m_ptr_[21]); }


        private:
            pointer m_ptr_;
    };

    Board();

    size_t& operator()(const size_t r, const size_t c);
    const size_t& operator()(const size_t r, const size_t c) const;



    SquareView operator[](const size_t sq);

    size_t sq(const size_t r, const size_t c) const;

    bool is_available(const size_t r, const size_t c, const size_t v) const;

    bool set(const size_t r, const size_t c, const size_t v);
    bool set(const size_t idx, const size_t v);

    friend ostream & operator << (ostream &, const Board &);

    bitset<9> availability(const size_t r, const size_t c) const;
    size_t count_availables(const size_t r, const size_t c) const;
    size_t count_availables(const size_t idx) const;

    void rand_set(const size_t r, const size_t c);

    void rand_set(const size_t idx);

    void rand();



// private:

    static size_t rc2idx(const size_t r, const size_t c);
    static pair<size_t, size_t> idx2rc(const size_t idx);

    vector<size_t> b_;

    vector<bitset<9>> flags_r_;
    vector<bitset<9>> flags_c_;
    vector<bitset<9>> flags_sq_;

    std::random_device dev_;
    std::mt19937 rng_;

};

