main: src/main.cpp
	g++ -c src/main.cpp -o build/main.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall
macos: src/main.cpp
	g++ -c src/main.cpp -o build/*.o -I ~/homebrew/include
	g++ build/*.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall -L ~/homebrew/lib
run:
	bin/bitten-adventure.exe
run-unix: bin/bitten-adventure
	bin/bitten-adventure
makeandrun: src/main.cpp
	g++ -c src/main.cpp -o build/*.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall
	bin/bitten-adventure
windows: src/main.cpp
	x86_64-w64-mingw32-g++ src/main.cpp -o build/*.o
	x86_64-w64-mingw32-g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall
