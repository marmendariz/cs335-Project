CFLAGS = -I ./include
LIB    = ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids bump fight

asteroids: asteroids.cpp ppm.c log.c
	g++ $(CFLAGS) asteroids.cpp log.c -Wall -Wextra $(LFLAGS) -o asteroids

bump: bump.cpp
	g++ bump.cpp -Wall -Wextra -obump -lX11 -lGL -lGLU -lm -lrt ./libggfonts.so

fight: fight.cpp
	g++ fight.cpp -Wall -Wextra -ofight -lX11 -lGL -lGLU -lm -lrt ./libggfonts.so

clean:
	rm -f asteroids
	rm -f bump
	rm -f fight
	rm -f *.o

