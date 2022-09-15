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
#include <fstream>
#include "bittendef.h"
// the settings.h file is currently broken
//#include "settings.h"
typedef enum {
    MUSIC    = 0
} StorageData;
const char* ConvertDoubleToString(double value){
    std::stringstream ss ;
    ss << value;
    const char* str = ss.str().c_str();
    return str;
}
// Persistent storage functions
static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);
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
    // setup player sprite and bgm
    #ifdef notepadplusplusDebug
    Texture2D bitten = LoadTexture("../assets/bitten.png");
    Music bgm = LoadMusicStream("../assets/bitten.wav");
    PlayMusicStream(bgm);
    #endif
    #ifndef notepadplusplusDebug
    Texture2D bitten = LoadTexture("assets/bitten.png");
    Music bgm = LoadMusicStream("assets/bitten.wav");
    PlayMusicStream(bgm);
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
    //load settings file, should be in a class but eh dont got time
    bool audio = true;
    int music =LoadStorageValue(0);
    if (music==0){
        audio=false;
        PauseMusicStream(bgm);
    }
    
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        
        if (title){
            if (IsKeyReleased(KEY_ENTER)) title=false;
            if (IsKeyReleased(KEY_M) & audio) {
                PauseMusicStream(bgm);
                audio=false;
                SaveStorageValue(MUSIC, 0);
            }
            else if (IsKeyReleased(KEY_M)){
                PlayMusicStream(bgm);
                audio=true;
                SaveStorageValue(MUSIC, 1);
            }
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
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM

    CloseAudioDevice(); 
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(SETTINGS_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", SETTINGS_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(SETTINGS_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", SETTINGS_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", SETTINGS_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(SETTINGS_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", SETTINGS_FILE, value);
    }

    return success;
}

int LoadStorageValue(unsigned int position)
{
    int value = 0;
    char ascii = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(SETTINGS_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", SETTINGS_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
            ascii << value;
        }

        UnloadFileData(fileData);
        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i(%a)", SETTINGS_FILE, value, ascii);
    }

    return value;
}