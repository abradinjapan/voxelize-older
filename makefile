debug:
	gcc src/main.c -Wall -fsanitize=address -o bin/voxelize-debug -lSDL2 -lGL -lGLEW -lm

release:
	gcc src/main.c -Wall -o bin/voxelize -lSDL2 -lGL -lGLEW -lm
