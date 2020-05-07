cmake_minimum_required(VERSION 2.8)
project(vertex_cover)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pthread ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## add additional .cpp files if needed
add_executable(vertex_cover solve_vertex_cover.cpp)
# link MiniSAT libraries
target_link_libraries(vertex_cover minisat-lib-static)

# link MiniSAT libraries
# target_link_libraries(test minisat-lib-static)
