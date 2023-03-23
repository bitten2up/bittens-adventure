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
// Bitten Patch                                           //
////////////////////////////////////////////////////////////
// Loads patches from dlls (if on windows)                //
////////////////////////////////////////////////////////////
// How I would recomend you to use                        //
// this is to load other dlls                             //
////////////////////////////////////////////////////////////
// TODO: FIX WINDOWS.H RECTANGLE AND RAYLIB.H RECTANGLE
// FROM CONFLECTING
////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dlfcn.h>

//#include "raylib.h"
#include "bit_patch.h"
#include "bittendef.h"

/* Declare imported function so that we can actually use it. */
void patch(bit_game* game)
{
    int (*patch_main)(bit_game*);
    void *handle;
    char *error;
    handle = dlopen(PATCH_DLL, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        printf("yeah you might want to fix your patch, it did nothing\n did you rember to name your function patch_main\n");
    }
    dlerror();
    patch_main = dlsym(handle, "patch_main");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(1);
    }
    (*patch_main)(game);
    dlclose(handle);
    return;
}