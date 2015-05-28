CFLAGS = -I ./include
LIB    = ./lib/fmod/libfmodex64.so ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
FLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm

all: fight 

fight: audio.o KevinO.o MarkA.o SarahbethR.o fight.o fmod.c ppm.c ZenaidaG.o 
	g++ $(CFLAGS) -o fight fight.o audio.o KevinO.o MarkA.o SarahbethR.o ZenaidaG.o fmod.c  ppm.c -Wall -Wextra $(LFLAGS)  

SarahbethR.o: SarahbethR.cpp
	g++ $(CFLAGS)  -c SarahbethR.cpp fmod.c ppm.c $(FLAGS)  

KevinO.o: KevinO.cpp
	g++ $(CFLAGS) -c KevinO.cpp fmod.c ppm.c $(FLAGS)

audio.o: audio.cpp fmod.c
	g++ $(CFLAGS) audio.cpp fmod.c ppm.c -Wall -c -Wextra -lX11 -lGL -lGLU -lm -lrt

fight.o: fight.cpp  ppm.c fmod.c
	g++ $(CFLAGS) fight.cpp fmod.c -Wall -Wextra -c -lX11 -lGL -lGLU -lm -lrt
	
MarkA.o: MarkA.cpp 
	g++ $(CFLAGS) -c MarkA.cpp fmod.c ppm.c -Wall -Wextra -lx11 -lGl -lGLU -lm -lrt

ZenaidaG.o: ZenaidaG.cpp
	g++ $(CFLAGS) -c ZenaidaG.cpp fmod.c ppm.c -Wall -Wextra -lx11 -lGl -lGLU -lm -lrt 

clean:
	rm -f fight
	rm -f *.o
 
