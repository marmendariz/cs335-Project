CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: fight 

fight: fight.o audio.o animations.o fmod.c ppm.c 
	g++ $(CFLAGS) -o fight fight.o audio.o animations.o fmod.c  ppm.c -Wall -Wextra $(LFLAGS)  

animations.o: animations.cpp
	g++ $(CFLAGS) -c animations.cpp fmod.c ppm.c $(LFLAGS)

audio.o: audio.cpp fmod.c
	g++ $(CFLAGS) audio.cpp fmod.c ppm.c -Wall -Wextra -c -lX11 -lGL -lGLU -lm -lrt

fight.o: fight.cpp  ppm.c fmod.c
	g++ $(CFLAGS) fight.cpp fmod.c -Wall -Wextra -c -lX11 -lGL -lGLU -lm -lrt



clean:
	rm -f fight
	rm -f *.o
 
