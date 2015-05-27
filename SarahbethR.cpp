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
	drawmenu_button
 */

#include <cstdio>
#include "SarahbethR.h"
#include "ZenaidaG.h"
#include "defs.h"

extern int xres, yres;
int z=1;
int leftButtonDown = 0;
Vec leftButtonPos;

extern Player play1, play2;
extern bool play_game, go_selchar, two_players, player1choose, player2choose;
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
backgroundBox1, 
backgroundBox2,
logoBox1, 
charPrompt1box, 
charPrompt2box, 
promptBox1, 
promptBox2;
//titleBox;

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

Ppmimage *promptBox1Image=NULL;
int prompt1=1;
GLuint promptBox1Texture;

Ppmimage *promptBox2Image=NULL;
int prompt2=1;
GLuint promptBox2Texture;

/*Ppmimage *titlePromptImage=NULL;
int titlePrompt=1;
GLuint titlePromptTexture;
*/
extern Ppmimage *titleImage;
extern int title;
extern GLuint titleTexture;
extern int play_sounds;
extern int selchar;
extern GLuint selectTexture;

extern clock_t startTime;
extern bool readyPrompt, readyPromptClk;

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
	

	////////////////////////////////////////////////////////
	
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
	
	   /*********** Initialize Textures **************/
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

	/*char m[] = "./images/titlePrompt.ppm"; 
	titlePromptImage = ppm6GetImage(titlePromptImage, m);
	glGenTextures(1, &titlePromptTexture);
	glBindTexture(GL_TEXTURE_2D, titlePromptTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = titlePromptImage->width;
	h = titlePromptImage->height;
	unsigned char *titlePromptData = buildAlphaData(titlePromptImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, titlePromptData);
	delete [] titlePromptData;

	 	GUILE LOGO TEXTURE	

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


	backgroundBox1.pos[0] = 900;
	backgroundBox1.pos[1] = 450;
	backgroundBox1.width = 200;
	backgroundBox1.height = 100; 

	backgroundBox2.pos[0] = 900;
	backgroundBox2.pos[1] = 250;
	backgroundBox2.width = 200;
	backgroundBox2.height = 100; 


	charPrompt1box.pos[0] = (xres/2);
	charPrompt1box.pos[1] = 180;
	charPrompt1box.width = 300;
	charPrompt1box.height = 80;

	charPrompt2box.pos[0] = (xres/2);
	charPrompt2box.pos[1] = 180;
	charPrompt2box.width = 300;
	charPrompt2box.height = 80;

	promptBox1.pos[0] = 0;
	promptBox1.pos[1] = yres/2;
	promptBox1.width = 200;
	promptBox1.height = 90;

	promptBox2.pos[0] = 0;
	promptBox2.pos[1] = yres/2;
	promptBox2.width = 300;
	promptBox2.height = 150;

	/*titleBox.pos[0] = 1050;
	titleBox.pos[1] = 460;
	titleBox.width = 150;
	titleBox.height = 80;*/

}

void selectBox(Vec leftButtonPos)
	{
    if (play_game == false) { // if in title screen
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
    glEnd();

 /*if (x == 9) { 
	    glBindTexture(GL_TEXTURE_2D, titlePromptTexture);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);

	glBegin(GL_QUADS);
    
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
    }
    glEnd();*/

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
		int w = 1050; 
		int y = 460; 
		/***************************************/ 
		/*Draw title screen background*/ 
		glColor3f(1.0, 1.0, 1.0); 
		if (title) { 
				glBindTexture(GL_TEXTURE_2D, titleTexture); 
				glBegin(GL_QUADS); 
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0); 
				glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres); 
				glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres); 
				glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0); 
				//ndTexture(GL_TEXTURE_2D, promptBox2Texture);
		}


		/*Draw the play button*/ 
		glColor3f(1.0,1.0,1.0); 
		/*****/ 
		glPushMatrix(); 
		glTranslatef(w,y,0); 
		//drawCharBox(titleBox.width, titleBox.height, 9);
		drawmenu_button(150,80); 
		glPopMatrix(); 
		/******************************************************/ 
} 

void drawmenu_button(Flt width, Flt height)
{
     int w = width, h = height;
 	
	//DRAW FIGHT BUTTON/
	
/*	glBindTexture(GL_TEXTURE_2D, titlePromptTexture); 
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.1f);
	glColor4ub(255,255,255,255);
	
	glBegin(GL_QUADS);
    
	   	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	    	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	    	glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	    	glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
	glEnd(); 
*/		

     glBegin(GL_QUADS);
     glColor3f(0.0,0.5,0.0);
     glVertex2i(-w, -h);
     glColor3f(0.0,0.0,1.0);
     glVertex2i(-w, h);
     glColor3f(0.0,0.5,0.0);
     glVertex2i(w, h);
     glColor3f(0.0,0.0,1.0);
     glVertex2i(w, -h);
     glEnd();


	/*glEnable(GL_TEXTURE_2D);
	 Rect r;
	 r.center = 1;
	 r.bot = 460;
	 r.left = 1050;
	 ggprint16(&r, 16, 0x2EFEC8, "CLICK TO PLAY!");*/

}


void animate_ReadySetGo()
{
	
	
	float ReadyTimeDif = float(clock() - startTime) / CLOCKS_PER_SEC; 
	printf("%f\n", ReadyTimeDif);	
	int inc = 0;

	/************** READY ***************/

	if(ReadyTimeDif >= 0.005 && ReadyTimeDif < 0.010)
		inc = 20;
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


