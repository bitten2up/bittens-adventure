
linux: src/main.cpp
	g++ -c src/main.cpp -o build/main.o
	g++ build/main.o -o bin/bitten-adventure -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall
windows: src/main.cpp
	windres icon.rc -O coff -o build/icon.res
	windres bitten.rc -O coff -o build/bitten.res
	x86_64-w64-mingw32-g++ src/main.cpp -c -o build/main.o -I/c/raylib/raylib/src/
	x86_64-w64-mingw32-g++ build/main.o build/bitten.res build/icon.res -o bin/bitten-adventure -L/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm -Wall
