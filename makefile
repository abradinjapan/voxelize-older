debug:
	gcc src/main.c -Wall -fsanitize=address -o voxelize -lSDL2 -lGL -lGLEW
