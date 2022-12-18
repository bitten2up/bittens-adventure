////////////////////////////////////////////////////////////
// Dll testing                                            //
////////////////////////////////////////////////////////////
// Sample dll for patching                                //
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "bittendef.h"

int __declspec(dllexport) __cdecl bittenEnginePatch(bit_gamestate* game) {
    game->settings->width=900;
    game->settings->silent=true; // dont show modded text
    printf("Bitten engine patching worked\nscreen width is now: %i", game->settings->width);
    return;
}