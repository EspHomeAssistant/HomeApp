cmake_minimum_required(VERSION 3.20)

project(HomeApp LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)

option(BUILD_TESTS "Builds tests" ON)
option(WITH_GDB "Adds -ggdb flag" ON)

if (WITH_GDB)
    add_compile_options("-ggdb")
endif()

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

# set(CMAKE_CXX_FLAGS "-static-libstdc++")
set(CMAKE_CXX_FLAGS "-static-libstdc++ -fconcepts-diagnostics-depth=2")

add_subdirectory(src)

if (BUILD_TESTS)
enable_testing()
add_subdirectory(test)
endif()
