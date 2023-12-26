cmake_minimum_required(VERSION 3.20)

project(HomeApp LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

option(BUILD_TESTS "Builds tests" ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

set(CMAKE_CXX_FLAGS "-static-libstdc++")

add_subdirectory(src)

if (BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif()
