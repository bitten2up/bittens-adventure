////////////////////////////////////////////////////////////
// Dll testing                                            //
////////////////////////////////////////////////////////////
// Sample dll for patching                                //
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "bittendef.h"

int __declspec(dllexport) __cdecl bittenEnginePatch(bit_settings* settings) {
    settings->width=900;
    settings->silent=true; // dont show modded text
    printf("Bitten engine patching worked\nscreen width is now: %i", settings->width);
    return 1;
}