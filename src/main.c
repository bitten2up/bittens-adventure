/*
MIT License

Copyright (c) 2022 bitten2up

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
#include <string.h>
#include <time.h>
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
// Discord RPC headers
////////////////////////////////////////////////////////////

#ifdef DISCORD
#include <discord_rpc.h>
//#include "discord_game_sdk.h"
#endif

////////////////////////////////////////////////////////////
// some stuff needed for Discord RPC
// feel free to hide this if your ide supports it
////////////////////////////////////////////////////////////

#ifdef DISCORD
static const char* APPLICATION_ID = "905202859686129784";
static int64_t StartTime;
static int SendPresence = 1;

static void updateDiscordPresence(char* message)
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Bittens adventure";
    discordPresence.details = message;
    discordPresence.startTimestamp = 1507665886;
    discordPresence.endTimestamp = 1507665886;
    discordPresence.largeImageKey = "window";
    discordPresence.largeImageText = "bittens-adventure";
    discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
    Discord_UpdatePresence(&discordPresence);
}


static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
           connectedUser->username,
           connectedUser->discriminator,
           connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordUser* request)
{
    int response = -1;
    char yn[4];
    printf("\nDiscord: join request from %s#%s - %s\n",
           request->username,
           request->discriminator,
           request->userId);
        response = DISCORD_REPLY_NO;
    if (response != -1) {
        Discord_Respond(request->userId, response);
    }
}

static void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

#endif


////////////////////////////////////////////////////////////
// Entrypoint of Engine
////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    // check command line paramiters to see if we need to exit or not because of a command line parm (should be in main.c but I'm trying to keep this file not cluttered as it it)
    int startup = cmdlineParams(argc, argv);
    if (startup==0)   { return 1; }
    else if (startup==2)    { patch(0); }
    // init the window
    InitWindow(SCREENWIDTH, SCREENHEIGHT, GAME_NAME);
    InitAudioDevice();
    #ifdef debugsprites
    SetTargetFPS(20);
    #endif
    #ifndef debugsprites
    SetTargetFPS(60);               // we want our game running at 60 fps to avoid audio skipping
    #endif
    // set window icon
    SetWindowIcon(LoadImage("assets/window.png"));
    // setup player sprite
    Texture2D bitten = LoadTexture("assets/bitten.png");
    bitten.width=bitten.width;
    bitten.height=bitten.height;
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
    Texture2D enemySprite = LoadTexture("assets/enemy.png");
    enemySprite.width=enemySprite.width*2;
    enemySprite.height=enemySprite.height*2;
    Rectangle enemyRec;
    enemyRec.width = enemySprite.width/2;
    enemyRec.height = enemySprite.height/4;
    Vector2 enemyPos;
    enemyPos.x = SCREENWIDTH/2 + SCREENWIDTH/3 - enemyRec.width/2;
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
    tmx_map* map = LoadTMX("assets/maps/bit_towntest.tmx");
    int lastx=0;
    int lasty=0;
    bool collision = false;
    // lastly in our setting up, setup discord rpc
    #ifdef DISCORD
    TraceLog(LOG_INFO, "Discord RPC activating");
    discordInit();
    TraceLog(LOG_INFO, "Discord RPC activated");
    updateDiscordPresence("Discord rpc testing");
    TraceLog(LOG_INFO, "Discord RPC status set");
    #endif
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        if (title){
            if (IsKeyReleased(KEY_TAB))     patch(0);
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
                y=lasty;
                #ifdef DISCORD
                updateDiscordPresence("Overworld");
                #endif
            }
        }
        
        else if (!battle & !title){
            if (IsKeyDown(KEY_RIGHT)){
                lastx=x;
                x -= 1;
            }
            if (IsKeyDown(KEY_LEFT)){
                lastx=x;
                x += 1;
            }
            if (IsKeyDown(KEY_UP)){
                lasty=y;
                y += 1;
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
                y -= 1;
                bittenRec.x = bitten.width/2;
                ticker+=1;
                if (ticker==5)
                {
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }/*
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
                if (audio)          PlayMusicStream(bgm);
            }*/
            if ((x < -178 && x > -230) && (y>-180 && y<-130)) {
                battle=true;
                enemy = "Generator";
                enemyHP=0;
                TraceLog(LOG_DEBUG, "ENGINE: ENTERING BATTLE: %s hp: %i", enemy, enemyHP);
                bittenPos.x = SCREENWIDTH/4- bittenRec.width/2;
                bittenPos.x = SCREENHEIGHT/4 - bittenRec.height/2;
                UnloadMusicStream(bgm);
                bgm=LoadMusicStream("assets/M_IntroHP.mp3");
                if (audio)          PlayMusicStream(bgm);
                #ifdef DISCORD
                char buf[20];
                sprintf(buf, "battling %s", enemy);
                updateDiscordPresence(buf);
                #endif
            }//*/
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
    #ifdef DISCORD
    Discord_Shutdown();
    #endif
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM
    CloseAudioDevice(); 
    UnloadTMX(map);
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}