main: src/main.cpp
	g++ -c src/main.cpp -o build/main.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall -lc -lSystem -lc++ -Wall -nodefaultlibs
macos: src/main.cpp
	g++ -c src/main.cpp -o build/*.o -I ~/homebrew/include
	g++ build/*.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lc -lSystem -lc++ -Wall -nodefaultlibs -L ~/homebrew/lib
run:
	bin/bitten-adventure.exe
run-unix: bin/bitten-adventure
	bin/bitten-adventure
makeandrun: src/main.cpp
	g++ -c src/main.cpp -o build/*.o
	g++ build/main.o -o bin/bitten-adventure -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall
	bin/bitten-adventure
windows: src/main.cpp
	/usr/bin/x86_64-w64-mingw32-windres icon.rc -O coff -o build/icon.res
	/usr/bin/x86_64-w64-mingw32-windres bitten.rc -O coff -o build/bitten.res
	x86_64-w64-mingw32-g++ src/main.cpp -c -o build/*.o -I/usr/x86_64-w64-mingw32/SFML-2.5.1/include
	x86_64-w64-mingw32-g++ build/main.o build/bitten.res build/icon.res -o bin/bitten-adventure -L/usr/x86_64-w64-mingw32/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lvorbis -Wall -lc -lSystem -lc++ -Wall -nodefaultlibs
