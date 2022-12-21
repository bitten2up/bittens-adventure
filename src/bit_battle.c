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
// Battle                                                 //
////////////////////////////////////////////////////////////
// Code for battle system                                 //
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// STD And Raylib
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <raylib.h>

////////////////////////////////////////////////////////////
// Discord RPC
////////////////////////////////////////////////////////////

#ifdef DISCORD
#include "discord_rpc.h"
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
// Engine headers
////////////////////////////////////////////////////////////
#ifdef DEFINE_MAIN
#undef DEFINE_MAIN
#endif
#include "bittendef.h"
#include "bit_battle.h"
#include "bit_diag.h"


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
// battle display
////////////////////////////////////////////////////////////
int frame=0;
bool grow=true;
static void battleIntro();
static void battleAttack();
bool bit_BattleDraw(bit_game* game)
{
    if (battleAni==intro)
    {
        battleIntro(game);
        return false;
    }
    diagDraw(true, &game->settings);
    if ((battleAni==playerAttack) | (battleAni==enemyAttack)) // tmp
    {
        battleAttack(game);
    }
    // draw the text, to be implmented into diagDraw
    if (enemyHealth<=0 && battleAni==waitInput)     DrawText("Well That was easy", 190, 200, 20, BLACK);
    DrawText("Bitten", game->settings.width/4, game->settings.height/3*2.5, 10, WHITE);
    DrawText(game->enemy.name, game->settings.width/4*3, game->settings.height/3*2.5, 10, WHITE);
    char working[5];
    ftoa(bittenHealth, working, 4);
    DrawText(working, game->settings.width/4, game->settings.height/3*2.6, 10, WHITE);
    ftoa(enemyHealth, working, 4);
    DrawText(working, game->settings.width/4*3, game->settings.height/3*2.6, 10, WHITE);
    return true;
}

// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
static void battleIntro(bit_game* game)
{
    const int wframe=((frame*5));
    if (wframe==(game->settings.height)){
        grow=false;
    }

	if (wframe<=game->settings.height/3 && !grow)
	{
	    battleAni=waitInput;
        grow=true;
        frame=0;
	}
   // battleAni=waitInput;
    if (grow)                        {DrawRectangle(0, game->settings.height-wframe, game->settings.width, game->settings.height, BLACK);frame+=1;}
    else                             {DrawRectangle(0, game->settings.height-wframe, game->settings.width, game->settings.height, BLACK);frame-=1;}
	return;
}

////////////////////////////////////////////////////////////
// battle attack
////////////////////////////////////////////////////////////

static void battleAttack(bit_game* game)
{
    // tmp player attack animation aka a fucking knife
    if (battleAni==playerAttack) {
        if (bittenHealth>0){
            const int wframe=((frame*5));
            if (wframe>=(game->settings.width/5)){
                grow=false;
                enemyHealth-=10;
            }
	        if (wframe==0 && !grow)
	        {
	            battleAni=enemyAttack;
                grow=true;
                frame=0;
	        }
            if (grow)                        {DrawRectangle(game->settings.width-wframe, 0, game->settings.width, game->settings.height, GRAY);frame+=1;}
            else                             {DrawRectangle(game->settings.width-wframe, 0, game->settings.width, game->settings.height, RED);frame-=1;}
        }
        else {
            battleAni=waitInput;
            grow=true;
            frame=0;
        }
    }
    // tmp enemy attack animation
    if (battleAni==enemyAttack) {
        if (enemyHealth>0){
            const int wframe=((frame*5));
            if (wframe>=(game->settings.width/5)){
                grow=false;
                bittenHealth-=10;
            }
	        if (wframe==0 && !grow)
	        {
	            battleAni=waitInput;
                grow=true;
                frame=0;
	        }
            if (grow)                        {DrawRectangle(0, 0, wframe, game->settings.height, GRAY);frame+=1;}
            else                             {DrawRectangle(0, 0, wframe, game->settings.height, RED);frame-=1;}
        }
        else {
            battleAni=waitInput;
            grow=true;
            frame=0;
        }
    }
	return;
}
////////////////////////////////////////////////////////////
// battle input
////////////////////////////////////////////////////////////

bool bit_battleInput(bit_game* game)
{
    if (bittenHealth==0)        {state=gameover; battleAni=intro; return true;} // tmp until x any y cords are in in bit_game
    if (IsKeyReleased(KEY_X) && enemyHealth==0 && battleAni==waitInput)
    {
        state=overworld;
        battleAni=intro;
        return true; // we don't want to run the rest of the code
    }
    else if (IsKeyReleased(KEY_X) && battleAni==waitInput) {
        grow=true;
        battleAni=playerAttack;
        return false;
    }
    return false;
}
////////////////////////////////////////////////////////////
// battle test
////////////////////////////////////////////////////////////
// pov delet this
////////////////////////////////////////////////////////////

void bit_BattleTest()
{
}
