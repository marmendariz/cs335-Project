all: bump

bump: bump.cpp
	g++ bump.cpp -Wall -Wextra -obump -lX11 -lGL -lGLU -lm -lrt ./libggfonts.so

clean:
	rm -f bump
	rm -f *.o

