
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
#include "audio.h"
#include "animations.h"
#include "init.h"

void init_character_boxes(void);
void selectBox(Vec);
void drawCharBox(Flt, Flt, int);
void character_select_render(void);
