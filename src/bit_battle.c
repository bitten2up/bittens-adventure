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
    if (enemyHealth<=0)     DrawText("Well That was easy", 190, 200, 20, BLACK);
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
    // tmp enemy attack animation
    if (battleAni==enemyAttack) {
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
	return;
}
////////////////////////////////////////////////////////////
// battle input
////////////////////////////////////////////////////////////

bool bit_battleInput(bit_game* game)
{
    if (bittenHealth==0)        {state=gameover; battleAni=intro; return true;} // tmp until x any y cords are in in bit_game
    if (IsKeyReleased(KEY_X) && enemyHealth==0)
    {
        state=overworld;
        battleAni=intro;
        return true; // we don't want to run the rest of the code
    }
    else if (IsKeyReleased(KEY_X)) {
        grow=true;
        battleAni=playerAttack;
        return false;
    }
    return false;
}
////////////////////////////////////////////////////////////
// battle test
////////////////////////////////////////////////////////////
// function for testing the battle mode. Mostly a lighter
// weight version of int main() with only the
// stuff for battles
////////////////////////////////////////////////////////////

void bit_BattleTest()
{/*
    InitWindow(game->settings.width, game->settings.height, "bittens adventure BATTLE TEST");
    // set window icon
    SetWindowIcon(LoadImage("assets/window.png"));
    SetTargetFPS(30);
    char* enemy;
    float enemyHP;
    float playerHP = 200;
    TraceLog(LOG_INFO, "ENGINE: You have loaded into the battle test, if you didn't mean to enter this mode close this window and relaunch the program");
    // setup player sprite
    Texture2D bitten = LoadTexture("assets/bitten.png");
    Rectangle bittenRec;
    bittenRec.width = bitten.width/2;
    bittenRec.height = bitten.height/3;
    // position of player
    Vector2 bittenPos;
    bittenPos.x = game->settings.width/2 - bittenRec.width/2;
    bittenPos.y = game->settings.height/2 - bittenRec.height;
    bittenRec.x = 2*bitten.width/2;
    bittenRec.y = 3*bitten.height/3;
    // ah this is defined when we go in battle, but rn i dont want to add this in the function
    enemy = "Dummy";
    enemyHP=0;
    bittenPos.x = game->settings.width/4- bittenRec.width/2;
    bittenPos.x = game->settings.height/4 - bittenRec.height/2;
    while (!WindowShouldClose())
    {
        // to be replaced with a function, but for now its ok
        if (IsKeyReleased(KEY_X))   return;
        BeginDrawing();
            ClearBackground(WHITE);
            DrawTextureRec(bitten,bittenRec,bittenPos,WHITE);
            bit_BattleDraw(&playerHP, &enemy, &enemyHP);
            DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(bitten);
    CloseWindow();
    */
}
