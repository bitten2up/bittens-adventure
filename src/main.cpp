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
#include <iostream>
#include <sstream>
#include "bittendef.h"
// the settings.h file is currently broken
//#include "settings.h"
const char* ConvertDoubleToString(double value){
    std::stringstream ss ;
    ss << value;
    const char* str = ss.str().c_str();
    return str;
}
int main(){
    // intinalization
    // TODO make this editable in an settings menu
    const int screenWidth = 800;
    const int screenHeight = 450;
    // init the window
    InitWindow(screenWidth, screenHeight, "bittens adventure");
    InitAudioDevice();  
    #ifdef debugsprites
    SetTargetFPS(15);
    #endif
    #ifndef debugsprites
    SetTargetFPS(60);               // we want our game running at 60 fps
    #endif
    // set window icon
    #ifdef notepadplusplusDebug
    SetWindowIcon(LoadImage("../assets/window.png"));
    #endif
    #ifndef notepadplusplusDebug
    SetWindowIcon(LoadImage("assets/bitten.png"));
    #endif
    // setup player sprite
    #ifdef notepadplusplusDebug
    Texture2D bitten = LoadTexture("../assets/bitten.png");
    Music music = LoadMusicStream("../assets/M_IntroHP.ogg");
    #endif
    #ifndef notepadplusplusDebug
    Texture2D bitten = LoadTexture("assets/bitten.png");
    Music music = LoadMusicStream("assets/M_IntroHP.ogg");
    PlayMusicStream(music);
    #endif
    Rectangle bittenRec;
    bittenRec.width = bitten.width/2;
    bittenRec.height = bitten.height/3;
    // position of player
    Vector2 bittenPos;
    bittenPos.x = screenWidth/2 - bittenRec.width/2;
    bittenPos.y = screenHeight/2 - bittenRec.height;
    bittenRec.x = 2*bitten.width/2;
    bittenRec.y = 3*bitten.height/3;
    // define some vars
    bool title=true;
    bool battle=false;
    char* enemy;
    double enemyHP;
    double playerHP = 200;
    int frame = 4;
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        
        if (title){
            if (IsKeyReleased(KEY_ENTER)) title=false;
        }
        else if (battle){
            if (IsKeyReleased(KEY_X))   battle=false; //will be updated later
        }
        if (!battle & !title){
            if (IsKeyDown(KEY_RIGHT)) bittenPos.x += 2;
            if (IsKeyDown(KEY_LEFT)) bittenPos.x -= 2;
            if (IsKeyDown(KEY_UP)){
                bittenPos.y -= 2;
                bittenRec.x = 2*bitten.width/2;
                frame+=1;
                bittenRec.y=frame*bitten.height/3;
            }
            if (IsKeyDown(KEY_DOWN)) {
                bittenPos.y += 2;
                bittenRec.x = bitten.width/2;
                bittenRec.y=3*bitten.height/3;
            }
            if (IsKeyReleased(KEY_X)){
                battle=true;
                enemy = "Dummy";
                enemyHP=0;
                bittenPos.x = screenWidth/4- bittenRec.width/2;
                bittenPos.x = screenHeight/4 - bittenRec.height/2;
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            if (title) DrawText("bitten's adventure", 190, 200, 20, BLACK);
            DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
            if (battle){
                DrawText("Well That was easy", 190, 200, 20, BLACK);
                DrawText(enemy, screenWidth/4*3, screenHeight/4*3, 10, BLACK);
                DrawText(ConvertDoubleToString(playerHP), screenWidth/4, screenHeight/4*3, 10, BLACK);
                DrawText(ConvertDoubleToString(enemyHP), screenWidth/4*3, screenHeight/4*3.1, 10, BLACK);
            }
        EndDrawing();
    }
    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    CloseAudioDevice(); 
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}