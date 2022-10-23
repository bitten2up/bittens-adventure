////////////////////////////////////////////////////////////
// Pointers test                                          //
////////////////////////////////////////////////////////////
// a file that helps me get better at pointers            //
// before using pointer in the engine.                    //
////////////////////////////////////////////////////////////
#include <stdlib.h>

////////////////////////////////////////////////////////////
// entry point of test
////////////////////////////////////////////////////////////
void testFunction(int *pointer)
{
    printf("Value of  pointer %d;\n", pointer);
    printf("Value of value taken from pointer %d;\n", *pointer);
    return;
}
int main()
{
    int value=0xB3B2;
    int *pointer;
    pointer = &value;
    printf("Value of  pointer %d;\n", pointer);
    printf("Value of value taken from pointer %d;\n", *pointer);
    printf("Entering Phase 2, changing value\n\n");
    value=0;
    printf("Value of  pointer %d;\n", pointer);
    printf("Value of value taken from pointer %d;\n", *pointer);
    printf("Entering Phase 3, running in sepreat function\n\n");
    testFunction(pointer);
    free(pointer) //memory safty
}