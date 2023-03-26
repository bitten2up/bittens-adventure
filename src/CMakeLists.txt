find_package(tmx REQUIRED)
find_package(raylib 4.5 REQUIRED) # Requires at least version 3.0

set(CMAKE_C_STANDARD 11) # Requires C11 standard

add_executable(bittens-adventure main.c  bit_cmdlineParams.c  bit_battle.c  bit_diag.c   bit_patch.c  bit_collision.c  bit_file.c)

target_link_libraries(bittens-adventure tmx raylib)
target_link_libraries(bittens-adventure m dl)
# The following libs should be transitively imported by raylib...
find_package(Threads REQUIRED)
target_link_libraries(bittens-adventure Threads::Threads)
if(UNIX)
  find_package(X11 REQUIRED)
  target_link_libraries(bittens-adventure X11::X11)
endif()

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(bittens-adventure "-framework IOKit")
    target_link_libraries(bittens-adventure "-framework Cocoa")
    target_link_libraries(bittens-adventure "-framework OpenGL")
endif()