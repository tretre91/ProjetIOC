cmake_minimum_required(VERSION 3.14)

project(PROJET_IOC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(testStableSolver "test/testStableSolver.cpp" "src/graph2.cpp" "src/weightedMaximumStableSolver.cpp" "src/stableSolver.cpp" "src/graphNO.cpp" )
target_include_directories(testStableSolver PRIVATE "src")
