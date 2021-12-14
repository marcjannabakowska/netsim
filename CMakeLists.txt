cmake_minimum_required(VERSION 3.21)
project(net_sim)

set(CMAKE_CXX_STANDARD 14)
include_directories(
        include
)

add_executable(net_sim main.cpp src/package.cpp
        include/factory.hpp include/reports.hpp include/simulation.hpp
        include/helpers.hpp include/nodes.hpp include/package.hpp
        include/storage_types.hpp include/types.hpp src/storage_types.cpp
        src/nodes.cpp src/helpers.cpp src/factory.cpp
        src/reports.cpp src/simulation.cpp)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)