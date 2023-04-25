cmake_minimum_required(VERSION 3.5)

project(bittens-adventure VERSION 1.0.0 LANGUAGES C)

if(MSVC AND NOT VCPKG_TOOLCHAIN)
  message(FATAL_ERROR "MSVC is only supported when using vcpkg")
endif()

add_executable(bittens-adventure "b_main.c" "r_render.c" "i_event.c" "f_save.c" "e_collision.c")

find_package(tmx REQUIRED)
target_link_libraries(bittens-adventure PRIVATE tmx)

if(VCPKG_TOOLCHAIN)
  #find_package(sdl2 REQUIRED) # transitively included?
  find_package(sdl2-image REQUIRED)
  target_link_libraries(bittens-adventure PRIVATE sdl2-image)
else()
  find_package(PkgConfig REQUIRED)
  #pkg_search_module(SDL2 REQUIRED IMPORTED_TARGET sdl2 SDL2_image) # does not work for unknown reasons
  pkg_search_module(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image) # transitively includes SDL2
  target_link_libraries(bittens-adventure PRIVATE PkgConfig::SDL2_IMAGE)
endif()
set(DISCORD OFF CACHE BOOL ON)
if (DISCORD)
    target_link_libraries(bittens-adventure PRIVATE discord-rpc)
    target_link_libraries(bittens-adventure PRIVATE stdc++)
    add_definitions(-DDISCORD)
endif()
