#!/bin/bash
# for the dll
gcc -o ../patch.dll -s -shared dll_testing.c -ggdb --enable-checking -g -O0 -v -da -Q -Wl,--subsystem,windows;
