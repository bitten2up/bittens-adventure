#include "raylib.h"

#define RAYLIB_TMX_IMPLEMENTATION
#include "raylib-tmx.h"

int main() {
    InitWindow(800, 450, "[raylib-tmx] example");

    tmx_map* map = LoadTMX("assets/maps/bit_test.tmx");

    while(!WindowShouldClose()) {

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTMX(map, 0, 0, WHITE);
        }
        EndDrawing();
    }

    UnloadTMX(map);

    CloseWindow();
    return 0;
}