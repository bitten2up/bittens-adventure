#!/bin/bash
# for the dll
gcc -o ../patch.dll -s -shared dll_testing.c -ldl;
