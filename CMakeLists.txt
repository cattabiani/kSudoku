cmake_minimum_required(VERSION 3.11.0)
project(kSudoku VERSION 0.1.0 LANGUAGES C CXX)


include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(kSudoku main.cpp ${all_SRCS})
