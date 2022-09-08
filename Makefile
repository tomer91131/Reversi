all: compile link
compile:
	g++ -I src/include -c  Tile.cpp Arti.cpp Game.cpp main.cpp
link:
	g++ main.o -o  main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system