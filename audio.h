#include "defs.h"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
extern "C" {
#include "fonts.h"
}

#define USE_SOUND

#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include "fmod.h"
#endif
void init_music();
void init_sound();
void soundeffects(int num);
void let_the_music_play(int num);

void goodbye();


