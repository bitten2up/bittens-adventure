#!/bin/bash

gcc -o patch.dll -s -shared dll_testing.c -lraylib -L/c/raylib/raylib/raylib  -Wl,--subsystem,windows;