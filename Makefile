all: snake.cpp
	g++ -I. -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio snake.cpp -o snake.exe