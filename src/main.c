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
// MAIN file                                              //
////////////////////////////////////////////////////////////
// Handles logic                                          //
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD and Raylib
////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <raylib.h>
#define RAYLIB_TMX_IMPLEMENTATION
#include "raylib-tmx.h"

////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "bittendef.h"           // defines for the engine
#include "bit_cmdlineParams.h"   // command line functionality
#include "bit_loadfile.h"        // file loading functionality
#include "bit_battle.h"          // battle functionality
#include "bit_patch.h"

////////////////////////////////////////////////////////////
// Entrypoint of Engine
////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    // check command line paramiters to see if we need to exit or not because of a command line parm
    int startup = cmdlineParams(argc, argv);
    if (startup==0)   { return 1; }
    else if (startup==2)    { patch(); }
    // init the window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "bittens adventure");
    InitAudioDevice();
    #ifdef debugsprites
    SetTargetFPS(20);
    #endif
    #ifndef debugsprites
    SetTargetFPS(60);               // we want our game running at 60 fps to avoid audio skipping
    #endif
    bool patched = patch();
    // set window icon
    SetWindowIcon(LoadImage("assets/window.png"));
    // setup player sprite
    Texture2D bitten = LoadTexture("assets/bitten.png");
    bitten.width=bitten.width*2;
    bitten.height=bitten.height*2;
    Rectangle bittenRec;
    bittenRec.width = bitten.width/2;
    bittenRec.height = bitten.height/4;
    // position of player
    Vector2 bittenPos;
    bittenPos.x = SCREENWIDTH/2 - bittenRec.width/2;
    bittenPos.y = SCREENHEIGHT/2 - bittenRec.height;
    bittenRec.x = 2*bitten.width/2;
    bittenRec.y = 3*bitten.height/4;
    // Enemy sprite loading
    Texture2D enemySprite;
    enemySprite.width=enemySprite.width*2;
    enemySprite.height=enemySprite.height*2;
    Rectangle enemyRec;
    enemyRec.width = enemySprite.width/2;
    enemyRec.height = enemySprite.height/4;
    Vector2 enemyPos;
    enemyPos.x = SCREENWIDTH/2 - enemyRec.width/2;
    enemyPos.y = SCREENHEIGHT/2 - enemyRec.height;
    enemyRec.x = 2*enemySprite.width/2;
    enemyRec.y = 3*enemySprite.height/4;
    // setup music
    Music bgm = LoadMusicStream("assets/bitten.wav");
    PlayMusicStream(bgm);
    // define some vars
    bool title=true;
    bool battle=false;
    char* enemy;
    float enemyHP;
    float playerHP = 200;
    int ticker = 0;
    int frame = 4;
    int x = 0;
    int y = 0;
    //load save file, should be in a function but eh dont got time
    bool audio = true;
    #ifndef PLATFORM_WEB
    if (LoadStorageValue(MUSIC)==0){ // for somereason writing a bool set to true saves as a 0
        audio=false;
        PauseMusicStream(bgm);
    }
    if (LoadStorageValue(SAVEDX))       x=LoadStorageValue(SAVEDX);
    if (LoadStorageValue(SAVEDY))       y=LoadStorageValue(SAVEDY);
    #endif
    // setup map
    TraceLog(LOG_INFO, "FILEIO: LOADING MAP");
    tmx_map* map = LoadTMX("assets/maps/bit_test.tmx");
    int lastx=0;
    int lasty=0;
    bool collision = false;
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        if (title){
            if (IsKeyReleased(KEY_TAB))     patch();
            if (IsKeyReleased(KEY_ENTER)) title=false;
            if (IsKeyReleased(KEY_M) & audio) {
                StopMusicStream(bgm);
                audio=false;
                #ifndef PLATFORM_WEB
                SaveStorageValue(MUSIC, 0);
                #endif
            }
            else if (IsKeyReleased(KEY_M)){
                PlayMusicStream(bgm);
                audio=true;
                SaveStorageValue(MUSIC, 1);
            }
            if (IsKeyPressed(KEY_F)){
                int display = GetCurrentMonitor();
                if (!IsWindowFullscreen()){
                    SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
                    ToggleFullscreen();
                }
                else {
                    ToggleFullscreen();
                    SetWindowSize(SCREENWIDTH, SCREENHEIGHT);
                }
            }
        }
        else if (battle){
            if (bit_battleInput(&battle, &enemyHP)){
                bittenPos.x = SCREENWIDTH/2 - bittenRec.width;
                bittenPos.y = SCREENHEIGHT/2 - bittenRec.height;
                x=lastx;
                UnloadTexture(enemySprite);
            }
        }
        
        else if (!battle & !title){
            if (IsKeyDown(KEY_RIGHT)){
                lastx=x;
                x -= 4;
            }
            if (IsKeyDown(KEY_LEFT)){
                lastx=x;
                x += 4;
            }
            if (IsKeyDown(KEY_UP)){
                lasty=y;
                y += 4;
                bittenRec.x = 2*bitten.width/2;
                ticker+=1;
                if (ticker==5)
                {
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_DOWN)) {
                lasty=y;
                y -= 4;
                bittenRec.x = bitten.width/2;
                ticker+=1;
                if (ticker==5)
                {
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (x>=4){
                battle=true;
                enemy = "Dummy";
                enemyHP=0;
                TraceLog(LOG_DEBUG, "ENGINE: ENTERING BATTLE: %s hp: %i", enemy, enemyHP);
                bittenPos.x = SCREENWIDTH/4- bittenRec.width/2;
                bittenPos.x = SCREENHEIGHT/4 - bittenRec.height/2;
                LoadTexture("assets/bitten.png");
            }
            if (y==4){
                battle=true;
                enemy = "Generator";
                enemyHP=100;
                TraceLog(LOG_DEBUG, "ENGINE: ENTERING BATTLE: %s hp: %i", enemy, enemyHP);
                bittenPos.x = SCREENWIDTH/4- bittenRec.width/2;
                bittenPos.x = SCREENHEIGHT/4 - bittenRec.height/2;
                UnloadMusicStream(bgm);
                bgm=LoadMusicStream("assets/M_IntroHP.mp3");
                LoadTexture("assets/bitten.png");
                if (audio)          PlayMusicStream(bgm);
            }
            if (IsKeyReleased(KEY_TAB)){
                SaveStorageValue(SAVEDX, x);
                SaveStorageValue(SAVEDY, y);
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            if (title) DrawText("bitten's adventure", 190, 200, 20, BLACK);
            else if (battle) {
                if (bit_BattleDraw(&playerHP, &enemy, &enemyHP)){
                    DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
                    DrawTextureRec(enemySprite, enemyRec, enemyPos, WHITE);
                }
            }
            else if (!battle) 
            {
                DrawTMX(map, x, y, WHITE);
                DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
                char xandy[20];
                snprintf(xandy, sizeof(xandy), "\nx: %i\ny: %i", x, y);
                DrawText(xandy, 20,10,20, BLACK);
            }
            DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM
    CloseAudioDevice(); 
    UnloadTMX(map);
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}
