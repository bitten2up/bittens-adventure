cmake_minimum_required(VERSION 3.15)
project(bittens-adventure)

add_subdirectory(src bittens-adventure)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})