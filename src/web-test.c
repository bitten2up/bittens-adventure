/*
MIT License

Copyright (c) 2023 bitten2up

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
#include <stdlib.h>
#include <math.h>
//#include "settings.h"

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
// Persistent storage functions
static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);
int main(int argc, char **argv){
    // intinalization
    // TODO make this editable in an settings menu
    const int SCREENWIDTH = 800;
    const SCREENHEIGHT = 450;
    // init the window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "bittens adventure");
    InitAudioDevice();
    #ifdef debugsprites
    SetTargetFPS(15);
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
    bittenPos.x = SCREENWIDTH/2 - bittenRec.width/2;
    bittenPos.y = SCREENHEIGHT/2 - bittenRec.height;
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
    int music =LoadStorageValue(0);
    if (music==0){
        audio=false;
        PauseMusicStream(bgm);
    }
    //free music somewhere here

    // setup map
    //TraceLog(LOG_INFO, "FILEIO: LOADING MAP");
    //tmx_map* map = LoadTMX("assets/maps/bit_test.tmx");
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);

        if (title){
            if (IsKeyReleased(KEY_ENTER)) title=false;
            if (IsKeyReleased(KEY_M) & audio) {
                StopMusicStream(bgm);
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

        else if (!battle & !title){
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
                bittenPos.x = SCREENWIDTH/4- bittenRec.width/2;
                bittenPos.x = SCREENHEIGHT/4 - bittenRec.height/2;
            }
        }
        BeginDrawing();
            ClearBackground(WHITE);
            if (title) DrawText("bitten's adventure", 190, 200, 20, BLACK);
            DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
            if (battle){
                DrawText("Well That was easy", 190, 200, 20, BLACK);
                DrawText(enemy, SCREENWIDTH/4*3, SCREENHEIGHT/4*3, 10, BLACK);
                char working[5];
                ftoa(playerHP, working, 4);
                DrawText(working, SCREENWIDTH/4, SCREENHEIGHT/4*3, 10, BLACK);
                ftoa(playerHP, working, 4);
                DrawText(working, SCREENWIDTH/4*3, SCREENHEIGHT/4*3.1, 10, BLACK);
            }
            //DrawTMX(map, 0, 0, WHITE);
        EndDrawing();
    }
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM
    CloseAudioDevice();
    //UnloadTmx(game.map);
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