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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <raylib.h>
#define RAYLIB_TMX_IMPLEMENTATION
#include "raylib-tmx.h"
#ifdef PLATFORM_WEB
    #include <emscripten/emscripten.h>
#endif

////////////////////////////////////////////////////////////
// ENGINE HEADERS
////////////////////////////////////////////////////////////

#include "bittendef.h"           // defines for the engine
#include "bit_cmdlineParams.h"   // command line functionality
#include "bit_loadfile.h"        // file loading functionality
#include "bit_battle.h"          // battle functionality
#include "bit_patch.h"           // dll patching
#include "bit_collision.h"       // colision handling
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
    time_t seconds;
    seconds = time(NULL);
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Bittens adventure";
    discordPresence.details = message;
    discordPresence.startTimestamp = seconds;
    discordPresence.endTimestamp = seconds+818;
    discordPresence.largeImageKey = "window";
    discordPresence.largeImageText = "bittens-adventure";
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
    pthread_t patching; // for patching
    // apply default settings
    bit_settings settings;
    settings.width=SCREENWIDTH;
    settings.height=SCREENHEIGHT;
    settings.audio=true;
    settings.modded=false;
    settings.silent=false;
    // check command line paramiters to see if we need to exit or not because of a command line parm (should be in main.c but I'm trying to keep this file not cluttered as it it)
    int startup = cmdlineParams(argc, argv);
    if (startup==0)   { return 1; }
    else if (startup==2)    { settings.modded=true; pthread_create(&patching, NULL, patch, &settings);}
    // init the window
    InitWindow(settings.width, settings.height, GAME_NAME);
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
    bittenRec.width = bitten.width/4;
    bittenRec.height = bitten.height/4;
    // position of player
    Vector2 bittenPos;
    bittenPos.x = settings.width/2 - bittenRec.width/3;
    bittenPos.y = settings.height/2 - bittenRec.height;
    bittenRec.x = 4*bitten.width/4;
    bittenRec.y = bitten.height/4;
    // Enemy sprite loading
    Texture2D enemySprite = LoadTexture("assets/enemy.png");
    enemySprite.width=enemySprite.width*2;
    enemySprite.height=enemySprite.height*2;
    Rectangle enemyRec;
    enemyRec.width = enemySprite.width/2;
    enemyRec.height = enemySprite.height/4;
    Vector2 enemyPos;
    enemyPos.x = settings.width/2 + settings.width/3 - enemyRec.width/2;
    enemyPos.y = settings.height/2 - enemyRec.height;
    enemyRec.x = 2*enemySprite.width/4;
    enemyRec.y = 3*enemySprite.height/4;
    // setup music
    Music bgm = LoadMusicStream("assets/bitten.wav");
    PlayMusicStream(bgm);
    // define some vars
    bool title=true;
    bool battle=false;
    /*
    char* enemy;
    float enemyHP;
    */
    bit_enemy enemy;
    float playerHP = 200;
    int ticker = 0;
    int frame = 4;
    int x = 0;
    int y = 0;
    //load save file, should be in a function but eh dont got time
    #ifndef PLATFORM_WEB
    if (LoadStorageValue(MUSIC)==0){ // for somereason writing a bool set to true saves as a 0
        settings.audio=false;
        PauseMusicStream(bgm);
    }
    if (LoadStorageValue(SAVEDX))       x=LoadStorageValue(SAVEDX);
    if (LoadStorageValue(SAVEDY))       y=LoadStorageValue(SAVEDY);
    #endif
    // setup map
    TraceLog(LOG_INFO, "FILEIO: LOADING MAP");
    #ifdef PLATFORM_WEB
    tmx_map* map = LoadTMX("assets/bit_towntest.tmx");
    #endif
    #ifndef PLATFORM_WEB
    tmx_map* map = LoadTMX("assets/maps/bit_towntest.tmx");
    #endif
    
    // last x and y to go back to
    int lastx=x;
    int lasty=y;
    // tile x and y
    int tilex;
    int tiley;
    //bool collision = false;
    int collision;//checkCollision(map, (x/32+map->width)/2, (y/32+map->width)/2);
    //TraceLog(LOG_INFO, "%i", collision);
    // lastly in our setting up, setup discord rpc
    #ifdef DISCORD
    TraceLog(LOG_DEBUG, "Discord RPC activating");
    discordInit();
    TraceLog(LOG_DEBUG, "Discord RPC activated");
    updateDiscordPresence("Discord rpc testing");
    TraceLog(LOG_DEBUG, "Discord RPC status set");
    #endif
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        if (title){
            if (IsKeyReleased(KEY_TAB))     {settings.modded=true; pthread_create(&patching, NULL, patch, &settings);}
            if (IsKeyReleased(KEY_ENTER)) title=false;
        }
        else if (battle){
            if (bit_battleInput(&battle, &enemy.hp)){
                bittenPos.x = settings.width/2 - bittenRec.width;
                bittenPos.y = settings.height/2 - bittenRec.height;
                x=(lastx);
                y=(lasty);
                #ifdef DISCORD
                updateDiscordPresence("Overworld");
                #endif
            }
            #ifdef DISCORD
            char buf[20];
            sprintf(buf, "battling %s", enemy);
            updateDiscordPresence(buf);
            #endif
        }
        
        else if (!battle & !title){
            if (IsKeyDown(KEY_RIGHT)){
                bittenRec.x = 3*bitten.width/4;
                ticker+=1;
                //TraceLog(LOG_INFO, "collision: %i", collision);
                //TraceLog(LOG_INFO,"tilex: %i", tilex);
                //TraceLog(LOG_INFO,"tiley: %i", tiley);
                if (ticker==5)
                {
                    lastx=x;
                    x -= 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_LEFT)){
                bittenRec.x = 2*bitten.width/4;
                ticker+=1;
                if (ticker==5)
                {
                    lastx=x;
                    x += 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_UP)){
                bittenRec.x = 2*bitten.width/2;
                ticker+=1;
                if (ticker==5)
                {
                    lasty=y;
                    y += 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_DOWN)) {
                bittenRec.x = bitten.width/4;
                ticker+=1;
                if (ticker==5)
                {
                    lasty=y;
                    y -= 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            
            tilex = (map->width/2)-((x)/32)-3; // dont ask me wtf this has to be subtracted by 3 idk
            tiley = (map->height/2)-((y+8)/32)-3; // dont ask me wtf this has to be subtracted by 3 idk
            //TraceLog(LOG_INFO,"tilex: %i", tilex);
            //TraceLog(LOG_INFO,"tiley: %i", tiley);
            //TraceLog(LOG_INFO, "collision: %i", collision);
            collision=checkCollision(map, tilex, tiley);
            if (collision==2) {
                battle=true;
                strcpy(enemy.name, "chest monster");
                //enemy = "chest monster";
                enemy.hp=0;
                TraceLog(LOG_DEBUG, "ENGINE: ENTERING BATTLE: %s hp: %i", enemy.name);
                bittenPos.x = settings.width/4- bittenRec.width/2;
                bittenPos.x = settings.height/4 - bittenRec.height/2;
                UnloadMusicStream(bgm);
                bgm=LoadMusicStream("assets/M_IntroHP.mp3");
                if (settings.audio)          PlayMusicStream(bgm);
            }
            if (IsKeyReleased(KEY_TAB)){
                SaveStorageValue(SAVEDX, x);
                SaveStorageValue(SAVEDY, y);
            }
        }
        if (IsKeyReleased(KEY_M) & settings.audio) {
            StopMusicStream(bgm);
            settings.audio=false;
            #ifndef PLATFORM_WEB
            SaveStorageValue(MUSIC, 0);
            #endif
        }
        else if (IsKeyReleased(KEY_M)){
            PlayMusicStream(bgm);
            settings.audio=true;
            SaveStorageValue(MUSIC, 1);
        }
        if (IsKeyPressed(KEY_F)){
            int display = GetCurrentMonitor();
            if (!IsWindowFullscreen()){
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display)); // todo fix teleportation that happens here
                settings.height=GetMonitorHeight(display);
                settings.width=GetMonitorWidth(display);
                ToggleFullscreen();
                bittenPos.x = settings.width/2 - bittenRec.width/3;
                bittenPos.y = settings.height/2 - bittenRec.height;
            }
            else {
                ToggleFullscreen();
                settings.width=SCREENWIDTH;
                settings.height=SCREENHEIGHT;
                SetWindowSize(settings.width, settings.height);
                bittenPos.x = settings.width/2 - bittenRec.width/3;
                bittenPos.y = settings.height/2 - bittenRec.height;
            }
        }
        if (!IsWindowFullscreen()){
            SetWindowSize(settings.width, settings.height);
        }
        
        BeginDrawing();
            ClearBackground(WHITE);
            if (title) DrawText("bitten's adventure", 190, 200, 20, BLACK);
            else if (battle) {
                if (bit_BattleDraw(&playerHP, &enemy, &settings)){
                    DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
                    DrawTextureRec(enemySprite, enemyRec, enemyPos, WHITE);
                }
            }
            else if (!battle) 
            {
                DrawTMX(map, x, y, WHITE);
                DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
                char xandy[20];
                snprintf(xandy, sizeof(xandy), "\nx: %i\ny: %i", tilex, tiley);
                DrawText(xandy, 20,10,20, BLACK);
            }
            if (settings.modded && title | !settings.silent)         DrawText("modded", 10, settings.height-20, 15, RED);
            DrawFPS(10, 10);
        EndDrawing();
    }
    #ifdef DISCORD
    Discord_Shutdown();
    #endif
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM
    CloseAudioDevice(); 
    //free_layers(chests);
    UnloadTMX(map);
    UnloadTexture(bitten);
    CloseWindow();
    return 0;
}