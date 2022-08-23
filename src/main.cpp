/*
MIT License

Copyright (c) 2022 Litten1up

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <raylib.h>
#include "bittendef.h"
// the settings.h file is currently broken
//#include "settings.h"

int main(){
    // intinalization
    // TODO make this editable in an settings menu
    const int screenWidth = 800;
    const int screenHeight = 450;
    // init the window
    InitWindow(screenWidth, screenHeight, "bittens adventure");
    SetTargetFPS(60);               // we want our game running at 60 fps
    // setup player sprite
    #ifdef notepadplusplusDebug
    Texture2D bitten = LoadTexture("../assets/bitten.png");
    #endif
    #ifndef notepadplusplusDebug
    Texture2D bitten = LoadTexture("assets/bitten.png");
    #endif
    Rectangle bittenRec;
    bittenRec.width = bitten.width;
    bittenRec.height = bitten.height;
    // position of player
    Vector2 bittenPos;
    bittenPos.x = screenWidth/2 - bittenRec.width/2;
    bittenPos.y = screenHeight/2 - bittenRec.height;
    // game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(WHITE);
            DrawText("bitten's adventure", 190, 200, 20, BLACK);
            DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
        EndDrawing();
    }
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}