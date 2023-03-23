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
// Command line Params                                    //
////////////////////////////////////////////////////////////
// reads command line params                              //
////////////////////////////////////////////////////////////

/// 0 for exit, 1 for continue execution, 2 for patching logic.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//#include "raylib.h"

#include "bittendef.h"
#include "bit_battle.h"

// this probly should BE int main(), but i dont feel like doing it rn even tho it would be easy
int cmdlineParams(int argc, char** argv){
    if (argc!=1){
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "--version")==0){
                printf("\nBitten Engine\n\nversion 1.0 indev\n\nfeatures subject to change\n\n\nCopyright (c) 2023 bitten2up, licened under the M.I.T license check LICENSE\n");
                return 0;
            }
            // easter eggs
            if (strcmp(argv[i], "--iced")==0){
                printf("cold bitten 0_0\n");
                return 0;
            }
            if (strcmp(argv[i], "--mental-stablity")==0){
                // damm you did me dirty, also why are you asking lol
                printf("nonexistent, im having a stroke 24/7\n\nWait why are you asking 0_0\nlike searisly\n");
                printf("Oh and don't even start with your excuses random stranger who is playing this game");
                return 0;
            }
            // legit dont spoil these hints, if this is ever popular enough to get spoiled hints i want the player to find this on their own
            if (strcmp(argv[i], "--up2bitten")==0){
                printf("Thats weird, sounds like a mirrored version of me");
                return 0;
            }
            // the battletest return, really you dont need to be here unless you are updating the battle system
            if (strcmp(argv[i], "--battletest")==0)
            {
                bit_BattleTest();
                return 0;
            }
            if (strcasecmp(argv[i], "--patch")==0){
                return 2;
            }
        }
    }
    return 1;
}