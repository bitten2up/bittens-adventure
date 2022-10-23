////////////////////////////////////////////////////////////
// Bitten Patch                                           //
////////////////////////////////////////////////////////////
// Loads patches from dlls (if on windows)                //
////////////////////////////////////////////////////////////
// How I would recomend you to use                        //
// this is to load other dlls                             //
////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "bit_patch.h"
//#include "raylib.h"
#ifdef _WIN32
#include <windows.h>
#include <winbase.h>
#include <windef.h>
typedef UINT (CALLBACK* BITPATCH)(void);
BITPATCH bitPatch;
DWORD dwParam1;
UINT  uParam2, uReturnVal;
#endif
/* Declare imported function so that we can actually use it. */
int patch(int consent)
{
    if (consent!=1){
        printf("dll patching option supplied, this could be dangerous due to the fact that it can do anything it wants to. use with causion\nclose this window now to cancel loading it otherwise press enter\n");
        system("pause");
    }
    #ifdef _WIN32
    int status = 0;
    HINSTANCE bitLibrary = LoadLibrary("patch.dll");
    if (bitLibrary != NULL)
    {
        bitPatch = (BITPATCH)GetProcAddress(bitLibrary, "bittenEnginePatch");
        if (!bitPatch){
            printf("yeah you might want to fix your patch, it did nothing\n did you rember to name your function bittenEnginePatch");
            FreeLibrary(bitLibrary);
        }
        else
        {
            printf("\n%i\n", bitPatch());
        }
        FreeLibrary(bitLibrary);
    }
    else {
        printf("Failed to load patch.dll");
    }
    return status;
    #endif
}