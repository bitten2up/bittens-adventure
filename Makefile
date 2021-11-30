main: src/main.cpp
	g++ -c src/main.cpp -o build/*.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall
run: bin/bitten-adventure.exe
	bin/bitten-adventure.exe
makeandrun: src/main.cpp
	g++ -c src/main.cpp -o build/*.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall
	bin/bitten-adventure.exe