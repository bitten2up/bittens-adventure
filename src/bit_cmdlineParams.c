// document in a little bit


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"

#include "bittendef.h"
#include "bit_battle.h"

// this probly should be part of int main(), but i dont feel like doing it rn even tho it would be easy
bool cmdlineParams(int argc, char** argv){
    if (argc!=1){
        for (int i = 1; i < argc; ++i) {
            if (strcmp(argv[i], "--version")==0){
                printf("\nBitten Engine\n\nversion 1.0 indev\n\nfeatures subject to change\n\n\nCopyright (c) 2022 Bitten2up, licened under the M.I.T license check LICENSE\n");
                return true;
            }
            if (strcmp(argv[i], "--iced")==0){
                printf("cold bitten 0_0\n");
                return true;
            }
            if (strcmp(argv[i], "battletest")) // return of the battle test
            {
                bit_BattleTest();
                return true;
            }
        }
    }
    return false;
}