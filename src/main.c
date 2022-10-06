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

#include "bittendef.h" // defines for the engine
#include "bit_loadfile.h" // file loading functionality
// settings.h for when  i go to fix that trainwreck of a file
//#include "settings.h"

////////////////////////////////////////////////////////////
// Just some conversion tools
////////////////////////////////////////////////////////////


// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
 
// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

////////////////////////////////////////////////////////////
// Entrypoint of Engine
////////////////////////////////////////////////////////////

int main(int argc, char **argv){
    // intinalization
    // TODO make this editable in an settings menu
    const int screenWidth = 800;
    const int screenHeight = 450;
    // init the window
    InitWindow(screenWidth, screenHeight, "bittens adventure");
    InitAudioDevice();
    #ifdef debugsprites
    SetTargetFPS(20);
    #endif
    #ifndef debugsprites
    SetTargetFPS(30);               // we want our game running at 30 fps
    #endif
    // set window icon
    SetWindowIcon(LoadImage("assets/window.png"));
    // setup player sprite
    Texture2D bitten = LoadTexture("assets/bitten.png");
    Rectangle bittenRec;
    bittenRec.width = bitten.width/2;
    bittenRec.height = bitten.height/3;
    // setup music
    Music bgm = LoadMusicStream("assets/bitten.wav");
    PlayMusicStream(bgm);
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
    float enemyHP;
    float playerHP = 200;
    int frame = 4;
    //load settings file, should be in a class but eh dont got time
    bool audio = true;
    #ifndef PLATFORM_WEB
    int music =LoadStorageValue(0);
    if (music==0){
        audio=false;
        PauseMusicStream(bgm);
    }
    //free music somewhere here
    #endif
    //free music somewhere here
    TraceLog(LOG_DEBUG, "FILEIO: Read music: %a", music);
    // setup map
    TraceLog(LOG_INFO, "FILEIO: LOADING MAP");
    tmx_map* map = LoadTMX("assets/maps/bit_test.tmx");
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        
        if (title){
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
        }
        else if (battle){
            if (IsKeyReleased(KEY_X))   battle=false; //will be updated ~~later~~ NOW
        }
        
        else if (!battle & !title){
            if (IsKeyDown(KEY_RIGHT)) x -= 4;
            if (IsKeyDown(KEY_LEFT))  x += 4;
            if (IsKeyDown(KEY_UP)){
                y += 4;
                bittenRec.x = 2*bitten.width/2;
                frame+=1;
                bittenRec.y=frame*bitten.height/3;
            }
            if (IsKeyDown(KEY_DOWN)) {
                y -= 4;
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
            if (!title) DrawTMX(map, x, y, WHITE);
            DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
            if (battle){
                DrawText("Well That was easy", 190, 200, 20, BLACK);
                DrawText(enemy, screenWidth/4*3, screenHeight/4*3, 10, BLACK);
                char working[5];
                ftoa(playerHP, working, 4);
                DrawText(working, screenWidth/4, screenHeight/4*3, 10, BLACK);
                ftoa(playerHP, working, 4);
                DrawText(working, screenWidth/4*3, screenHeight/4*3.1, 10, BLACK);
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