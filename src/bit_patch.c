////////////////////////////////////////////////////////////
// Bitten Patch                                           //
////////////////////////////////////////////////////////////
// Loads patches from dlls (if on windows)                //
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
int patch()
{
    #ifdef _WIN32
    int status = 0;
    HINSTANCE bitLibrary = LoadLibrary("patch.dll");
    if (bitLibrary != NULL)
    {
        bitPatch = (BITPATCH)GetProcAddress(bitLibrary, "bittenEnginePatch");
        if (!bitPatch){
            printf("yeah you might want to fix your patch, it did nothing");
            FreeLibrary(bitLibrary);
        }
        else
        {
            printf("%i", bitPatch());
        }
        FreeLibrary(bitLibrary);
    }
    else {
        printf("Failed to load patch.dll");
    }
    return status;
    #endif
}