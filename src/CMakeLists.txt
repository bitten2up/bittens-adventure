cmake_minimum_required(VERSION 3.5)

project(bittens-adventure VERSION 1.0.0 LANGUAGES C)

if(MSVC AND NOT VCPKG_TOOLCHAIN)
  message(FATAL_ERROR "MSVC is only supported when using vcpkg")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wpedantic -Wextra -std=c99 -fsanitize=address")

add_executable(bittens-adventure "main.c" "g_game.c" "b_battle.c" "p_player.c" "e_collision.c" "f_save.c" "i_event.c" "r_render.c")

find_package(tmx REQUIRED)
target_link_libraries(bittens-adventure PRIVATE tmx)

if(VCPKG_TOOLCHAIN)
  #find_package(sdl2 REQUIRED) # transitively included?
  find_package(sdl2-image REQUIRED)
  find_package(SDL_ttf REQUIRED)
  target_link_libraries(bittens-adventure PRIVATE sdl2-image)
else()
  find_package(PkgConfig REQUIRED)
  #pkg_search_module(SDL2 REQUIRED IMPORTED_TARGET sdl2 SDL2_image) # does not work for unknown reasons
  pkg_search_module(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image) # transitively includes SDL2
  target_link_libraries(bittens-adventure PRIVATE PkgConfig::SDL2_IMAGE)
  pkg_search_module(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf) # transitively includes SDL2
  target_link_libraries(bittens-adventure PRIVATE PkgConfig::SDL2_TTF)
endif()
set(DISCORD OFF CACHE BOOL ON)
if (DISCORD)
    target_link_libraries(bittens-adventure PRIVATE discord-rpc)
    target_link_libraries(bittens-adventure PRIVATE stdc++)
    add_definitions(-DDISCORD)
endif()
set(DEBUG OFF CACHE BOOL ON)
if (DEBUG) 
    add_definitions(-DDEBUG)
endif()
