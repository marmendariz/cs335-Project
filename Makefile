CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
FLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: fight 

fight: audio.o animations.o init.o characterSelect.o fight.o fmod.c ppm.c 
	g++ $(CFLAGS) -o fight fight.o audio.o animations.o init.o characterSelect.o fmod.c  ppm.c -Wall -Wextra $(LFLAGS)  

characterSelect.o: characterSelect.cpp
	g++ $(CFLAGS) -c characterSelect.cpp fmod.c ppm.c $(FLAGS)

animations.o: animations.cpp
	g++ $(CFLAGS) -c animations.cpp fmod.c ppm.c $(FLAGS)

audio.o: audio.cpp fmod.c
	g++ $(CFLAGS) audio.cpp fmod.c ppm.c -Wall -c -Wextra -lX11 -lGL -lGLU -lm -lrt

fight.o: fight.cpp  ppm.c fmod.c
	g++ $(CFLAGS) fight.cpp fmod.c -Wall -Wextra -c -lX11 -lGL -lGLU -lm -lrt
	
init.o: init.cpp 
	g++ $(CFLAGS) -c init.cpp fmod.c ppm.c -Wall -Wextra -lx11 -lGl -lGLU -lm -lrt

clean:
	rm -f fight
	rm -f *.o
 
