cmake_minimum_required(VERSION 3.14)

project(PROJET_IOC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(PROJECT_SOURCES
   "src/weightedMaximumStableSolver.hpp"
   "src/weightedMaximumStableSolver.cpp"
   "src/stableSolver.hpp"
   "src/stableSolver.cpp"
   "src/graph2.hpp"
   "src/graph2.cpp"
   "src/graphNO.hpp"
   "src/graphNO.cpp"
)

add_library(common INTERFACE)
target_sources(common INTERFACE ${PROJECT_SOURCES})
target_include_directories(common INTERFACE src)

add_executable(testStableSolver "test/testStableSolver.cpp")
target_link_libraries(testStableSolver PRIVATE common)

add_executable(testSolver "test/testHeuristic.cpp")
target_link_libraries(testSolver PRIVATE common)

add_executable(benchmark "test/benchmark.cpp")
target_link_libraries(benchmark PRIVATE common)
set_target_properties(benchmark PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE)
