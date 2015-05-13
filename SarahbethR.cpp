// Sarahbeth Ramirez
// CS 335 - Final Project
// Homework 6

/****************/
/* 	CONTAINS: 

    init_char_boxes
    draw_char_boxes
    select_box
    character_select_render

 */

#include <cstdio>
#include "SarahbethR.h"
#include "defs.h"

extern int xres, yres;
extern bool play_game, go_selchar, two_players;

/********** Structures ********/

typedef struct t_charBox
{
    float width, height;
    Vec pos;
} charBox;

charBox charBox1, charBox2, logoBox1;

/********* Declare Textures **********/

Ppmimage *guileImage=NULL;
int guile=1;
GLuint guileTexture;

Ppmimage *bguileImage=NULL;
int bguile=1;
GLuint bguileTexture;

Ppmimage *gLogoImage=NULL;
int glogo=1;
GLuint glogoTexture;

extern Ppmimage *selectcharacter_Image;
extern int selchar;
extern GLuint selectTexture;

/********** Functions *********/

void init_character_boxes(void)
{
    int w,h;

    glViewport(0, 0, xres, yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, xres, 0, yres, -1, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    /*********** Initialize Textures **************/
    /*GUILE CHARACTER BOX TEXTURE*/
    char b[] = "./images/selectGuile.ppm"; 
    guileImage = ppm6GetImage(guileImage, b);
    glGenTextures(1, &guileTexture);
    glBindTexture(GL_TEXTURE_2D, guileTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    w = guileImage->width;
    h = guileImage->height;
    unsigned char *guileData = buildAlphaData(guileImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, guileData);
    delete [] guileData;

    /* BIZARRO GUILE CHARACTER BOX TEXTURE */
	
	char c[] = "./images/selectBGuile.ppm";
	bguileImage = ppm6GetImage(bguileImage, c);
	glGenTextures(1, &bguileTexture);
	glBindTexture(GL_TEXTURE_2D, bguileTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = bguileImage->width;
	h = bguileImage->height;
	unsigned char *bguileData = buildAlphaData(bguileImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, bguileData);
	delete [] bguileData;
	
	/* 	GUILE LOGO TEXTURE	

	char d[] = "./images/guileLogo.ppm";
	glogoImage = ppm6GetImage(glogoImage, d);
	glGenTextures(1, &glogoTexture);
	glBindTexture(GL_TEXTURE_2D, glogoTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = */

	/********** Initialize Box Position (s) **********/
	
    charBox1.pos[0] = 200;
    charBox1.pos[1] = 450;
    charBox1.width = 100;
    charBox1.height = 150;
	
	charBox2.pos[0] = 500;
	charBox2.pos[1] = 450;
	charBox2.width = 100;
	charBox2.height = 150;
}

void selectBox(Vec leftButtonPos)
{
    if (play_game == false) { // if in title screen
        if (go_selchar==false && two_players==false){
            if ((leftButtonPos[0] > (xres/2 - 75) && leftButtonPos[0] < xres/2 + 75) && 
                    (leftButtonPos[1] > (yres/8 - 25) && leftButtonPos[1] < (yres/8 + 25))) 
                go_selchar = true;
        }
    }
}  

void drawCharBox(Flt width, Flt height, int x)
{
    int w = width;
    int h = height;

    glEnable(GL_TEXTURE_2D);
    
	// Draw Guile Character Box //
	if (x == 1) 
        glBindTexture(GL_TEXTURE_2D, guileTexture);
    		
	glBegin(GL_QUADS);
 	if (x == 1) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();
 
 	// Draw Bizarro Guile Character Box
 	if (x == 2) 
        glBindTexture(GL_TEXTURE_2D, bguileTexture);

    glBegin(GL_QUADS);
    if (x == 2) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void character_select_render(void)
{

    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw Character Select Background
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0, 1.0, 1.0);
    if (selchar) {
        glBindTexture(GL_TEXTURE_2D, selectTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    //draw Guile character box
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(charBox1.pos[0], charBox1.pos[1], 0);
    drawCharBox(charBox1.width, charBox1.height, 1);
    glPopMatrix();

	//draw BGuile character box
	glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(charBox2.pos[0], charBox2.pos[1], 0);
    drawCharBox(charBox2.width, charBox2.height, 2);
    glPopMatrix();

}

/**************** UNDER DEVELOPMENT **********************/

/*void animate_ReadySetGo(Flt width, Flt height)
{
	float timeDif = float(clock() - startTime) / CLOCKS_PER_SEC; 
	startClk = false;

	int w = width;
	int h = height;
	float x_val = 0.0f;
	float y_top = 0.0f;
	float y_bot = 1.0f/3.0f;
	float inc = 0.1f;

	if (timeDif < 2) {
		play1.control = false;
		play2.control = false;
	}
	if(timeDif < 1) 
		x_val = 0.1f;
	else if (timeDif >= 1 && timeDif < 2)
		x_val = 0.2f;
	else if (timeDif > 2) {
		play1.control = true;
		play2.control = true;
	}

*/	
