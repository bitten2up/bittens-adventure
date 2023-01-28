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
// Discord RPC headers
////////////////////////////////////////////////////////////

#ifdef DISCORD
#include <discord_rpc.h>
//#include "discord_game_sdk.h"
#endif

////////////////////////////////////////////////////////////
// some stuff needed for Discord RPC
////////////////////////////////////////////////////////////
// feel free to hide this if your ide supports it.
// its up here because we will redefine state
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
// ENGINE HEADERS
////////////////////////////////////////////////////////////
#define DEFINE_MAIN              // we are the main function, so we dont want defines for pointers
#include "bittendef.h"           // defines for the engine
#include "bit_cmdlineParams.h"   // command line functionality
#include "bit_battle.h"          // battle functionality
#include "bit_patch.h"           // dll patching
#include "bit_collision.h"       // colision handling
#include "rust/bit_save.h"       // saving


////////////////////////////////////////////////////////////
// Entrypoint of Engine
////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    pthread_t patching; // for patching
    // load superstruct (as im calling it)
    bit_game game;
    // apply default settings
    game.settings.width=SCREENWIDTH;
    game.settings.height=SCREENHEIGHT;
    game.settings.modded=false;
    game.settings.silent=false;
    game.invalidSave=false;
    state=title;
    battleAni=intro;
    // check command line paramiters to see if we need to exit or not because of a command line parm (should be in main.c but I'm trying to keep this file not cluttered as it it)
    int startup = cmdlineParams(argc, argv);
    if (startup==0)   { return 1; }
    else if (startup==2)    { game.settings.modded=true; pthread_create(&patching, NULL, patch, &game);}
    // init the window
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(game.settings.width, game.settings.height, GAME_NAME);
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
    // Texture2D bitten = LoadTexture("assets/bitten.png");
    bitten=LoadTexture("assets/bitten.png");
    bitten.width=bitten.width;
    bitten.height=bitten.height;
    bittenRec.width = bitten.width/4;
    bittenRec.height = bitten.height/4;
    // position of player
    bittenPos.x = game.settings.width/2 - bittenRec.width;
    bittenPos.y = game.settings.height/2 - bittenRec.height;
    bittenRec.x = 4*bitten.width/4;
    bittenRec.y = bitten.height/4;
    bittenDirection = up;
    // Enemy sprite loading
    #define enemySprite game.enemy.sprite.img
    enemySprite = LoadTexture("assets/enemy.png");
    enemySprite.width=enemySprite.width*2;
    enemySprite.height=enemySprite.height*2;
    #define enemyRec game.enemy.sprite.rec
    enemyRec.width = enemySprite.width/2;
    enemyRec.height = enemySprite.height/4;
    #define enemyPos game.enemy.sprite.pos  // position of sprite
    enemyPos.x = game.settings.width/2 + game.settings.width/3 - enemyRec.width/2;
    enemyPos.y = game.settings.height/2 - enemyRec.height;
    enemyRec.x = 2*enemySprite.width/4;
    enemyRec.y = 3*enemySprite.height/4;
    //load save file, should be in a function but eh dont got time
    loadGame(&game);
    // setup music
    Music bgm = LoadMusicStream("assets/bitten.wav");
    PlayMusicStream(bgm);
    if (!game.settings.audio)                StopMusicStream(bgm);
    int ticker = 0;
    int frame = 4;
    // last x and y to go back to
    int lastx=game.player.x;
    int lasty=game.player.y;
    // tile x and y
    int tilex;
    int tiley;
    // setup map
    TraceLog(LOG_INFO, "FILEIO: LOADING MAP");
    #ifdef PLATFORM_WEB
    tmx_map* map = LoadTMX("assets/bit_towntest.tmx");
    #endif
    #ifndef PLATFORM_WEB
    tmx_map* map = LoadTMX("assets/maps/bit_towntest.tmx");
    #endif
    // declare collision type
    int collision;
    // lastly in our setting up, setup discord rpc
    #ifdef DISCORD
    TraceLog(LOG_DEBUG, "Discord RPC activating");
    discordInit();
    TraceLog(LOG_DEBUG, "Discord RPC activated");
    updateDiscordPresence("Discord rpc testing");
    TraceLog(LOG_DEBUG, "Discord RPC status set");
    #endif
    // see if the game has an invalid save file
    if (game.invalidSave==true) {
        while (!WindowShouldClose) {
            BeginDrawing();
                if (isTitle) DrawText("Error invalid save file, file from newer version", 190, 200, 20, BLACK);
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
        return 1;
    }
    // game loop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        if ((isTitle) | (isGameover)){
            if (IsKeyReleased(KEY_TAB) && !game.settings.modded)     {
                game.settings.modded=true;
                pthread_create(&patching, NULL, patch, &game.settings);
            }

            if (IsKeyReleased(KEY_ENTER)) {
                bittenHealth=100;
                state=overworld;

            }
        }
        else if (isBattle){
            if (bit_battleInput(&game)){
                if (disableCollision(map, tilex, tiley)==1)
                {
                    game.player.x=(lastx);
                    game.player.y=(lasty);
                }
                bittenPos.x = game.settings.width/2 - bittenRec.width;
                bittenPos.y = game.settings.height/2 - bittenRec.height;
                #ifdef DISCORD
                updateDiscordPresence("Overworld");
                #endif
            }
            #ifdef DISCORD
            char buf[20];
            sprintf(buf, "battling %s", game.enemy.name);
            updateDiscordPresence(buf);
            #endif
        }

        else if (isOverworld){
            if (IsKeyDown(KEY_RIGHT)){
                bittenDirection=right;
                ticker+=1;
                //TraceLog(LOG_INFO, "collision: %i", collision);
                //TraceLog(LOG_INFO,"tilex: %i", tilex);
                //TraceLog(LOG_INFO,"tiley: %i", tiley);
                if (ticker==5)
                {
                    lastx=game.player.x;
                    game.player.x -= 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_LEFT)){
                bittenDirection=left;
                ticker+=1;
                if (ticker==5)
                {
                    lastx=game.player.x;
                    game.player.x += 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_UP)){
                bittenDirection=up;
                ticker+=1;
                if (ticker==5)
                {
                    lasty=game.player.y;
                    game.player.y += 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            if (IsKeyDown(KEY_DOWN)) {
                bittenDirection=down;
                ticker+=1;
                if (ticker==5)
                {
                    lasty=game.player.y;
                    game.player.y -= 8;
                    frame+=1;
                    bittenRec.y=frame*bitten.height/4;
                    ticker=0;
                }
            }
            tilex = (map->width/2)-((game.player.x)/32)-3; // dont ask me wtf this has to be subtracted by 3 idk
            tiley = (map->height/2)-((game.player.y+8)/32)-3; // dont ask me wtf this has to be subtracted by 3 idk
            collision=checkCollision(map, tilex, tiley);
            //TraceLog(LOG_INFO, "collision: %i", collision);
            if (collision==2) {
                state=battle;
                strcpy(game.enemy.name, "chest monster");
                //enemy = "chest monster";
                game.enemy.health=10;
                TraceLog(LOG_DEBUG, "ENGINE: ENTERING BATTLE: %s health: %i", game.enemy.name);
                bittenPos.x = game.settings.width/4 - bittenRec.width;
                bittenPos.y = game.settings.height-(game.settings.height/2);
                UnloadMusicStream(bgm);
                bgm=LoadMusicStream("assets/M_IntroHP.mp3");
                if (game.settings.audio)          PlayMusicStream(bgm);
            }
            if (IsKeyReleased(KEY_TAB)){
                // SaveStorageValue(SAVEDX, x);
                // SaveStorageValue(SAVEDY, y);
                saveGame(&game);
            }
        }
        if (IsKeyReleased(KEY_M) && game.settings.audio) {
            StopMusicStream(bgm);
            game.settings.audio=false;
            #ifndef PLATFORM_WEB
            //SaveStorageValue(MUSIC, 0);
            saveGame(&game);
            #endif
        }
        else if (IsKeyReleased(KEY_M)){
            PlayMusicStream(bgm);
            game.settings.audio=true;
            //SaveStorageValue(MUSIC, 1);
            saveGame(&game);
        }
        #ifdef BROKENCODETBH
        if (IsKeyPressed(KEY_F)){
            int display = GetCurrentMonitor();
            if (!IsWindowFullscreen()){
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display)); // todo fix teleportation that happens here
                game.settings.height=GetMonitorHeight(display);
                game.settings.width=GetMonitorWidth(display);
                ToggleFullscreen();
                bittenPos.x = game.settings.width/2 - bittenRec.width;
                bittenPos.y = game.settings.height/2 - bittenRec.height;
            }
            else {
                ToggleFullscreen();
                game.settings.width=SCREENWIDTH;
                game.settings.height=SCREENHEIGHT;
                SetWindowSize(game.settings.width, game.settings.height);
                bittenPos.x = game.settings.width/2 - bittenRec.width;
                bittenPos.y = game.settings.height/2 - bittenRec.height;
            }
        }
        #endif
        if (!IsWindowFullscreen()){
            SetWindowSize(game.settings.width, game.settings.height);
        }
        bittenRec.x = bittenDirection*bitten.width/4;
        bittenRec.y = bitten.height/4;

        BeginDrawing();
            ClearBackground(WHITE);
            if (isTitle) DrawText("bitten's adventure", 190, 200, 20, BLACK);
            if (isGameover) DrawText("GAMEOVER", 190, 200, 20, BLACK);
            else if (isBattle) {
                if (bit_BattleDraw(&game)){
                    DrawTextureRec(bitten, bittenRec,bittenPos,WHITE);
                    DrawTextureRec(enemySprite, enemyRec, enemyPos, WHITE);
                }
            }
            else if (isOverworld)
            {
                DrawTMX(map, game.player.x, game.player.y, WHITE);
                DrawTextureRec(bitten, bittenRec,bittenPos,WHITE);
                char xandy[20];
                snprintf(xandy, sizeof(xandy), "\nx: %i\ny: %i", tilex, tiley);
                DrawText(xandy, 20,10,20, BLACK);
            }
            if (game.settings.modded && (isTitle | !game.settings.silent))         DrawText("modded", 10, game.settings.height-20, 15, RED);
            DrawFPS(10, 10);
        EndDrawing();
    }
    saveGame(&game);
    #ifdef DISCORD
    Discord_Shutdown();
    #endif
    UnloadMusicStream(bgm);   // Unload bgm stream buffers from RAM
    CloseAudioDevice();
    //free_layers(chests);
    UnloadTMX(map);
    UnloadTexture(bitten);
    CloseWindow();
    TraceLog(LOG_INFO, "print memory leaks");
    return 0;
}