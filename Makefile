ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
    make windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
    make linux
endif

linux: src/main.cpp
	g++ -c src/main.cpp -o build/main.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall
windows: src/main.cpp
	/usr/bin/x86_64-w64-mingw32-windres icon.rc -O coff -o build/icon.res
	/usr/bin/x86_64-w64-mingw32-windres bitten.rc -O coff -o build/bitten.res
	x86_64-w64-mingw32-g++ src/main.cpp -c -o build/*.o -I/usr/x86_64-w64-mingw32/SFML-2.5.1/include
	x86_64-w64-mingw32-g++ build/main.o build/bitten.res build/icon.res -o bin/bitten-adventure -L/usr/x86_64-w64-mingw32/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall
