all: snake.cpp
	g++ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio snake.cpp -o snake.exe