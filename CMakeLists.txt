cmake_minimum_required(VERSION 3.15)
project(bittens-adventure)

add_subdirectory(src bittens-adventure)
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
target_include_directories(bittens-adventure PRIVATE include)
add_dependencies(bittens-adventure copy_assets)
