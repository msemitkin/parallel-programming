cmake_minimum_required(VERSION 3.21)
project(paralell_programming)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/Cellar/open-mpi/4.1.3/include)
add_executable(paralell_programming main.cpp)
