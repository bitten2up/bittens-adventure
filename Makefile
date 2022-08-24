ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
    make windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
    make linux
endif

linux: src/main.cpp
	g++ -c src/main.cpp -o build/main.o
	g++ build/main.o -o bin/bitten-adventure -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall
windows: src/main.cpp
	/usr/bin/x86_64-w64-mingw32-windres icon.rc -O coff -o build/icon.res
	/usr/bin/x86_64-w64-mingw32-windres bitten.rc -O coff -o build/bitten.res
	x86_64-w64-mingw32-g++ src/main.cpp -c -o build/*.o
	x86_64-w64-mingw32-g++ build/main.o build/bitten.res build/icon.res -o bin/bitten-adventure -lraylib -lopengl32 -lgdi32 -lwinmm -Wall
