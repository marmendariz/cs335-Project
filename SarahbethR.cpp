// Sarahbeth Ramirez
// CS 335 - Final Project
// Homework 6

/****************/
/* 	CONTAINS: 

    init_char_boxes
    draw_char_boxes
    select_box
    character_select_render
	menu_render
	check_mouse
 */

#include <cstdio>
#include "SarahbethR.h"
#include "audio.h"
#include "defs.h"
#include "ZenaidaG.h"

int z=1;
int leftButtonDown = 0;
Vec leftButtonPos;

extern bool help_screen;
extern int xres, yres;
extern int play_sounds;
extern int selchar;
extern int title;
extern Player play1, play2;
extern clock_t startTime;
extern bool readyPrompt, readyPromptClk;
extern bool play_game, go_selchar, two_players, player1choose, player2choose, selectedBack;
extern "C" {
	#include "fonts.h"
}

/********** Structures ********/

typedef struct t_charBox
{
    float width, height;
    Vec pos;
} charBox;

charBox 
charBox1, 
charBox2,
charBox3,
charBox4, 
backgroundBox1, 
backgroundBox2,
logoBox1, 
charPrompt1box, 
charPrompt2box,
charPrompt3box,  
promptBox1, 
promptBox2,
endBox1,
endBox2,
titleBox;

/********* Declare Textures **********/

Ppmimage *guileImage=NULL;
int guile=1;
GLuint guileTexture;

Ppmimage *bguileImage=NULL;
int bguile=1;
GLuint bguileTexture;

Ppmimage *jokerImage=NULL;
int joker=1;
GLuint jokerTexture;

Ppmimage *redImage=NULL;
int red=1;
GLuint redTexture;

Ppmimage *gLogoImage=NULL;
int glogo=1;
GLuint glogoTexture;

Ppmimage *CharStreetImage=NULL;
int CharStreet=1;
GLuint CharStreetTexture;

Ppmimage *CharForestImage=NULL;
int CharForest=1;
GLuint CharForestTexture;

Ppmimage *charPrompt1Image=NULL;
int charPrompt1=1;
GLuint charPrompt1Texture;

Ppmimage *charPrompt2Image=NULL;
int charPrompt2=1;
GLuint charPrompt2Texture;

Ppmimage *charPrompt3Image=NULL;
int charPrompt3=1;
GLuint charPrompt3Texture;

Ppmimage *promptBox1Image=NULL;
int prompt1=1;
GLuint promptBox1Texture;

Ppmimage *promptBox2Image=NULL;
int prompt2=1;
GLuint promptBox2Texture;

Ppmimage *titleBoxImage=NULL;
int titlePrompt=1;
GLuint titleBoxTexture;

/*******************************************************/
Ppmimage *play1winsImage=NULL;
int play1wins=1;
GLuint play1winsTexture;

Ppmimage *play2winsImage=NULL;
int play2wins=1;
GLuint play2winsTexture;

/********************************************/

extern Ppmimage *titleImage;
extern GLuint titleTexture;
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
	
	  /*JOKER CHARACTER BOX TEXTURE*/
    char n[] = "./images/selectJoker.ppm"; 
    jokerImage = ppm6GetImage(jokerImage, n);
    glGenTextures(1, &jokerTexture);
    glBindTexture(GL_TEXTURE_2D, jokerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    w = jokerImage->width;
    h = jokerImage->height;
    unsigned char *jokerData = buildAlphaData(jokerImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, jokerData);
    delete [] jokerData;

    /* BIZARRO GUILE CHARACTER BOX TEXTURE */
	
	char o[] = "./images/selectRed.ppm";
	redImage = ppm6GetImage(redImage, o);
	glGenTextures(1, &redTexture);
	glBindTexture(GL_TEXTURE_2D, redTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = redImage->width;
	h = redImage->height;
	unsigned char *redData = buildAlphaData(redImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, redData);
	delete [] redData;
	

	/* BACKGROUND 1 SELECT (STREET) */
	
	char d[] = "./images/charStreet.ppm";
	CharStreetImage = ppm6GetImage(CharStreetImage, d);
	glGenTextures(1, &CharStreetTexture);
	glBindTexture(GL_TEXTURE_2D, CharStreetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = CharStreetImage->width;
	h = CharStreetImage->height;
	unsigned char *CharStreetData = buildAlphaData(CharStreetImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, CharStreetData);
	delete [] CharStreetData;
	
	
	/* BACKGROUND 2 SELECT (FOREST) */

	char l[] = "./images/charForest.ppm";
	CharForestImage = ppm6GetImage(CharForestImage, l);
	glGenTextures(1, &CharForestTexture);
	glBindTexture(GL_TEXTURE_2D, CharForestTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = CharForestImage->width;
	h = CharForestImage->height;
	unsigned char *CharForestData = buildAlphaData(CharForestImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, CharForestData);
	delete [] CharForestData;
	

	/* PROMPT TO SELECT PLAYER 1 */
	
	char f[] = "./images/charPrompt1.ppm";
	charPrompt1Image = ppm6GetImage(charPrompt1Image, f);
	glGenTextures(1, &charPrompt1Texture);
	glBindTexture(GL_TEXTURE_2D, charPrompt1Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = charPrompt1Image->width;
	h = charPrompt1Image->height;
	unsigned char *charPrompt1Data = buildAlphaData(charPrompt1Image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, charPrompt1Data);
	delete [] charPrompt1Data;
	
	/* PROMPT TO SELECT PLAYER 2 */
	
	char g[] = "./images/charPrompt2.ppm";
	charPrompt2Image = ppm6GetImage(charPrompt2Image, g);
	glGenTextures(1, &charPrompt2Texture);
	glBindTexture(GL_TEXTURE_2D, charPrompt2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = charPrompt2Image->width;
	h = charPrompt2Image->height;
	unsigned char *charPrompt2Data = buildAlphaData(charPrompt2Image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, charPrompt2Data);
	delete [] charPrompt2Data;
	
	/* PROMPT TO SELECT BACKGROUND */
	
	char p[] = "./images/selectStage.ppm";
	charPrompt3Image = ppm6GetImage(charPrompt3Image, p);
	glGenTextures(1, &charPrompt3Texture);
	glBindTexture(GL_TEXTURE_2D, charPrompt3Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = charPrompt3Image->width;
	h = charPrompt3Image->height;
	unsigned char *charPrompt3Data = buildAlphaData(charPrompt3Image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, charPrompt3Data);
	delete [] charPrompt3Data;
	
	/***** ANIMATE READY FIGHT TEXTURES *********/  
	
	/*READY PROMPT BOX TEXTURE*/
	
	char j[] = "./images/ReadyPrompt.ppm"; 
	promptBox1Image = ppm6GetImage(promptBox1Image, j);
	glGenTextures(1, &promptBox1Texture);
	glBindTexture(GL_TEXTURE_2D, promptBox1Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = promptBox1Image->width;
	h = promptBox1Image->height;
	unsigned char *promptBox1Data = buildAlphaData(promptBox1Image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, promptBox1Data);
		delete [] promptBox1Data;
    
	/* FIGHT PROMPT TEXTURE */
	
	char i[] = "./images/FightPrompt.ppm"; 
	promptBox2Image = ppm6GetImage(promptBox2Image, i);
	glGenTextures(1, &promptBox2Texture);
	glBindTexture(GL_TEXTURE_2D, promptBox2Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = promptBox2Image->width;
	h = promptBox2Image->height;
	unsigned char *promptBox2Data = buildAlphaData(promptBox2Image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, promptBox2Data);
	delete [] promptBox2Data;

	
	/*****************************************************************/
	//title prompt (button that says FIGHT! in title screen)

	char m[] = "./images/titlePrompt.ppm"; 
	titleBoxImage = ppm6GetImage(titleBoxImage, m);
	glGenTextures(1, &titleBoxTexture);
	glBindTexture(GL_TEXTURE_2D, titleBoxTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = titleBoxImage->width;
	h = titleBoxImage->height;
	unsigned char *titleBoxData = buildAlphaData(titleBoxImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, titleBoxData);
	delete [] titleBoxData;




	/*************************************************************************/
	char play1[] = "./images/play1wins.ppm"; 
	play1winsImage = ppm6GetImage(play1winsImage, play1);
	glGenTextures(1, &play1winsTexture);
	glBindTexture(GL_TEXTURE_2D, play1winsTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = play1winsImage->width;
	h = play1winsImage->height;
	unsigned char *play1winsData = buildAlphaData(play1winsImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, play1winsData);
		delete [] play1winsData;
    
    	char play2[] = "./images/play2wins.ppm"; 
	play2winsImage = ppm6GetImage(play2winsImage, play2);
	glGenTextures(1, &play2winsTexture);
	glBindTexture(GL_TEXTURE_2D, play2winsTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = play2winsImage->width;
	h = play2winsImage->height;
	unsigned char *play2winsData = buildAlphaData(play2winsImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, play2winsData);
		delete [] play2winsData;
    


/*****************************************************************************/



	/* 	GUILE LOGO TEXTURE	

	char d[] = "./images/guileLogo.ppm";
	glogoImage = ppm6GetImage(glogoImage, d);
	glGenTextures(1, &glogoTexture);
	glBindTexture(GL_TEXTURE_2D, glogoTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = */

	/********** Initialize Box Position (s) **********/
	
    charBox1.pos[0] = 150;
    charBox1.pos[1] = 480;
    charBox1.width = 100;
    charBox1.height = 150;
	
	charBox2.pos[0] = 450;
	charBox2.pos[1] = 480;
	charBox2.width = 100;
	charBox2.height = 150;

	charBox3.pos[0] = 1050;
    charBox3.pos[1] = 480;
    charBox3.width = 100;
    charBox3.height = 150;
	
	charBox4.pos[0] = 750;
	charBox4.pos[1] = 480;
	charBox4.width = 100;
	charBox4.height = 150;

	backgroundBox1.pos[0] = 350;
	backgroundBox1.pos[1] = 480;
	backgroundBox1.width = 250;
	backgroundBox1.height = 120; 

	backgroundBox2.pos[0] = 950;
	backgroundBox2.pos[1] = 480;
	backgroundBox2.width = 250;
	backgroundBox2.height = 120; 

	charPrompt1box.pos[0] = (xres/2);
	charPrompt1box.pos[1] = 180;
	charPrompt1box.width = 300;
	charPrompt1box.height = 80;

	charPrompt2box.pos[0] = (xres/2);
	charPrompt2box.pos[1] = 180;
	charPrompt2box.width = 300;
	charPrompt2box.height = 80;

	charPrompt3box.pos[0] = (xres/2);
	charPrompt3box.pos[1] = 180;
	charPrompt3box.width = 300;
	charPrompt3box.height = 80;


	promptBox1.pos[0] = 0;
	promptBox1.pos[1] = yres/2;
	promptBox1.width = 200;
	promptBox1.height = 90;

	promptBox2.pos[0] = 0;
	promptBox2.pos[1] = yres/2;
	promptBox2.width = 300;
	promptBox2.height = 150;
	
	endBox1.pos[0] = 0;
	endBox1.pos[1] = yres/2;
	endBox1.width = 300;
	endBox1.height = 150;

	endBox2.pos[0] = 0;
	endBox2.pos[1] = yres/2;
	endBox2.width = 300;
	endBox2.height = 150;

	titleBox.pos[0] = 1050;
	titleBox.pos[1] = 460;
	titleBox.width = 150;
	titleBox.height = 80;

}

void selectBox(Vec leftButtonPos)
	{
    if (play_game == false) { 
        if (go_selchar==false && two_players==false){

            if ((leftButtonPos[0] > (1050 - 75) && leftButtonPos[0] < 1050 + 75) && 
                    (leftButtonPos[1] > (460 - 25) && leftButtonPos[1] < (460 + 25))) 

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
 
	// Draw joker Character Box //
	if (x == 10) 
        glBindTexture(GL_TEXTURE_2D, jokerTexture);
    		
	glBegin(GL_QUADS);
 	if (x == 10) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();
 
 	// Draw Bizarro Guile Character Box
 	if (x ==11) 
        glBindTexture(GL_TEXTURE_2D, redTexture);

    glBegin(GL_QUADS);
    if (x == 11) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();



	 //Draw Select Background 1 (street)
 	if (x == 3) 
        glBindTexture(GL_TEXTURE_2D, CharStreetTexture);

    glBegin(GL_QUADS);
    if (x == 3) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();
	

	 //Draw Select Background 2 (street)
 	if (x == 8) 
        glBindTexture(GL_TEXTURE_2D, CharForestTexture);

    glBegin(GL_QUADS);
    if (x == 8) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();
	
	// Draw Select Player 1 Prompt
 	if (x == 4) 
        glBindTexture(GL_TEXTURE_2D, charPrompt1Texture);
	
    glBegin(GL_QUADS);
    if (x == 4) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();

	// Draw Select Player 2 Prompt
 	if (x == 5) 
        glBindTexture(GL_TEXTURE_2D, charPrompt2Texture);
	
    glBegin(GL_QUADS);
    if (x == 5) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();

	// Draw Select BACKGROUND Prompt
 	if (x == 13) 
        glBindTexture(GL_TEXTURE_2D, charPrompt3Texture);
	
    glBegin(GL_QUADS);
    if (x == 13) {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();


/*****************************************************/
		// REQUIRE ALPHA

	// DRAW READY..... 
    if (x == 6) { 
	    glBindTexture(GL_TEXTURE_2D, promptBox1Texture);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
	// DRAW FIGHT! 

  if (x == 7) { 
	    glBindTexture(GL_TEXTURE_2D, promptBox2Texture);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }

 /************************************************************************/
 if (x == 14) { 
	    glBindTexture(GL_TEXTURE_2D, play1winsTexture);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }

 if (x == 15) { 
	    glBindTexture(GL_TEXTURE_2D, play2winsTexture);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }



/************************************************************************/    


// DRAW TITLE SCREEN FIGHT BOX

 	if (x == 9) { 
	    glBindTexture(GL_TEXTURE_2D, titleBoxTexture);
	if (x == 9) {	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
	}
    }
    glEnd();
	glDisable(GL_ALPHA_TEST);
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

	if (player2choose == false) {

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

	//draw Joker character box
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(charBox3.pos[0], charBox3.pos[1], 0);
    drawCharBox(charBox3.width, charBox3.height, 10);
    glPopMatrix();

	//draw RED character box
	glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(charBox4.pos[0], charBox4.pos[1], 0);
    drawCharBox(charBox4.width, charBox4.height, 11);
    glPopMatrix();

	}

	if (player2choose == true) {
	
	//draw street background box
	glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(backgroundBox1.pos[0], backgroundBox1.pos[1], 0);
    drawCharBox(backgroundBox1.width, backgroundBox1.height, 3);
    glPopMatrix();

	//draw street background box
	glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(backgroundBox2.pos[0], backgroundBox2.pos[1], 0);
    drawCharBox(backgroundBox2.width, backgroundBox2.height, 8);
    glPopMatrix();
	
	//if (player1choose == true && player2choose == true && selectedBack == false) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charPrompt3box.pos[0], charPrompt3box.pos[1], 0);
		drawCharBox(charPrompt3box.width, charPrompt3box.height, 13);
		glPopMatrix();	
	
	}
	
	//draw prompts
	
	if ( two_players == false && selchar == true && player1choose == false && player2choose == false) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charPrompt1box.pos[0], charPrompt1box.pos[1], 0);
		drawCharBox(charPrompt1box.width, charPrompt1box.height, 4);
		glPopMatrix();
	} else if ( two_players == false && selchar == true && player1choose == true && player2choose == false) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charPrompt2box.pos[0], charPrompt2box.pos[1], 0);
		drawCharBox(charPrompt2box.width, charPrompt2box.height, 5);
		glPopMatrix();
	}
}

void menu_render(void)  
{ 
	int x = 1050; 
	int y = 460; 
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw MENU Background
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0, 1.0, 1.0);
    if (title) {
        glBindTexture(GL_TEXTURE_2D, titleTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
    	glDisable(GL_TEXTURE_2D);

			//Draw the play button 
			glColor3f(1.0,1.0,1.0); 
			 
			glPushMatrix(); 
			glTranslatef(x,y,0); 
			//drawCharBox(titleBox.width, titleBox.height, 9);
			drawmenu_button(160,90); 
			glPopMatrix(); 
	
    	//draw TITLE FIGHT box
    	glColor3f(1.0,1.0,1.0);
    	glPushMatrix();
    	glTranslatef(titleBox.pos[0], titleBox.pos[1], 0);
    	drawCharBox(titleBox.width, titleBox.height, 9);
    	glPopMatrix();
	}

	if(help_screen ==true){
	glColor3f(1.0,1.0,1.0);
    	glPushMatrix();
    	glTranslatef(400, 500, 0);
    	drawCharBox(titleBox.width, titleBox.height, 9);
    	glPopMatrix();

	
	}
}

void drawmenu_button(Flt width, Flt height)
{
     int w = width, h = height;
 	
	//DRAW FIGHT BUTTON/
	
	/*	glBindTexture(GL_TEXTURE_2D, titlePromptTexture); 
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);*/
	
	glBegin(GL_QUADS);
    
	   	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    	glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    	glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
	glEnd(); 
		

     glBegin(GL_QUADS);
     glColor3f(0.0,0.0,0.0);
     glVertex2i(-w, -h);
     glColor3f(0.0,0.0,1.0);
     glVertex2i(-w, h);
     glColor3f(0.0,0.0,0.0);
     glVertex2i(w, h);
     glColor3f(0.0,0.0,1.0);
     glVertex2i(w, -h);
     glEnd();
}
/*
	glEnable(GL_TEXTURE_2D);
	 Rect r;
	 r.center = 1;
	 r.bot = 460;
	 r.left = 1050;
	 ggprint16(&r, 16, 0x2EFEC8, "CLICK TO PLAY!");*/

//}

int q=0;

void animate_ReadySetGo()
{
	
	
	float ReadyTimeDif = float(clock() - startTime) / CLOCKS_PER_SEC; 
	printf("%f\n", ReadyTimeDif);	
	int inc = 0;

	/************** READY ***************/

	if(ReadyTimeDif >= 0.005 && ReadyTimeDif < 0.010){
		inc = 20;
		if(q==0){
		play_sounds=0;
		soundeffects(16);
		q++;
		}
	}
	if(ReadyTimeDif >= 0.010 && ReadyTimeDif < 0.015)
		inc = 40;
	if(ReadyTimeDif >= 0.015 && ReadyTimeDif < 0.020)
		inc = 60;
	if(ReadyTimeDif >= 0.020 && ReadyTimeDif < 0.025)
		inc = 80;
	if(ReadyTimeDif >= 0.025 && ReadyTimeDif < 0.030)
		inc = 100;
	if(ReadyTimeDif >= 0.030 && ReadyTimeDif < 0.035)
		inc = 120;
	if(ReadyTimeDif >= 0.035 && ReadyTimeDif < 0.040)
		inc = 140;
	if(ReadyTimeDif >= 0.040 && ReadyTimeDif < 0.045)
		inc = 160;
	if(ReadyTimeDif >= 0.045 && ReadyTimeDif < 0.050)
		inc = 180;
	if(ReadyTimeDif >= 0.050 && ReadyTimeDif < 0.055)
		inc = 200;
	if(ReadyTimeDif >= 0.055 && ReadyTimeDif < 0.060)
		inc = 220;
	if(ReadyTimeDif >= 0.060 && ReadyTimeDif < 0.065)
		inc = 240;
	if(ReadyTimeDif >= 0.065 && ReadyTimeDif < 0.070)
		inc = 260;
	if(ReadyTimeDif >= 0.070 && ReadyTimeDif < 0.075)
		inc = 280;
	if(ReadyTimeDif >= 0.075 && ReadyTimeDif < 0.080)
		inc = 300;
	if(ReadyTimeDif >= 0.080 && ReadyTimeDif < 0.085)
		inc = 320;
	if(ReadyTimeDif >= 0.085 && ReadyTimeDif < 0.090)
		inc = 340;
	if(ReadyTimeDif >= 0.090 && ReadyTimeDif < 0.095)
		inc = 360;
	if(ReadyTimeDif >= 0.095 && ReadyTimeDif < 0.100)
		inc = 380;
	if(ReadyTimeDif >= 0.100 && ReadyTimeDif < 0.105)
		inc = 400;
	if(ReadyTimeDif >= 0.105 && ReadyTimeDif < 0.110)
		inc = 420;
	if(ReadyTimeDif >= 0.110 && ReadyTimeDif < 0.115)
		inc = 440;
	if(ReadyTimeDif >= 0.115 && ReadyTimeDif < 0.120)
		inc = 460;
	if(ReadyTimeDif >= 0.120 && ReadyTimeDif < 0.125)
		inc = 480;
	if(ReadyTimeDif >= 0.125 && ReadyTimeDif < 0.130)
		inc = 500;
	if(ReadyTimeDif >= 0.130 && ReadyTimeDif < 0.135)
		inc = 520;
	if(ReadyTimeDif >= 0.135 && ReadyTimeDif < 0.140)
		inc = 540;
	if(ReadyTimeDif >= 0.140 && ReadyTimeDif < 0.145)
		inc = 560;
	if(ReadyTimeDif >= 0.145 && ReadyTimeDif < 0.150)
		inc = 580;
	if(ReadyTimeDif >= 0.150 && ReadyTimeDif < 0.155)
		inc = 600;
	if(ReadyTimeDif >= 0.155 && ReadyTimeDif < 0.160)
		inc = 620;

	/************** READY HOVER *********************/

	if(ReadyTimeDif >= 0.160 && ReadyTimeDif < 0.260)
		inc = 640;

	if(ReadyTimeDif < 0.260){
    	glPushMatrix();
    	glTranslatef(promptBox1.pos[0] + inc, promptBox1.pos[1], 0);
    	drawCharBox(promptBox1.width, promptBox1.height, 6);
    	glPopMatrix();
	}
	
	/***************** FIGHT *************/

	if (ReadyTimeDif >= 0.260 && ReadyTimeDif < 0.320) {
		inc = (xres/2);
		if(q==1){
		play_sounds=0;
                soundeffects(17);
		q++;
		}
		glPushMatrix();
	 	glTranslatef(promptBox2.pos[0] + inc, promptBox2.pos[1], 0);
	    	drawCharBox(promptBox2.width, promptBox2.height, 7);
	    	glPopMatrix();
	}

	if(ReadyTimeDif >= 0.320){
		inc = 640;
		readyPrompt = false;
		readyPromptClk = true;
		play1.control = true;
		play2.control = true;
	} 

}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		leftButtonDown=0;
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			leftButtonDown = 1;
			leftButtonPos[0] = (Flt)e->xbutton.x;
			leftButtonPos[1] = (Flt)(yres - e->xbutton.y);
			selectBox(leftButtonPos);
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		if (leftButtonDown) {
			leftButtonPos[0] = (Flt)e->xbutton.x;
			leftButtonPos[1] = (Flt)(yres - e->xbutton.y);
		}
	}
}


