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


charBox charBox1, charBox2, backgroundBox1, logoBox1, charPrompt1box, charPrompt2box, promptBox1;

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


//Ppmimage *streetImage=NULL;
//int street=1;
//GLuint streetTexture;

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
	
	/*char d[] = "./images/street.ppm";
	streetImage = ppm6GetImage(streetImage, d);
	glGenTextures(1, &streetTexture);
	glBindTexture(GL_TEXTURE_2D, streetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = streetImage->width;
	h = streetImage->height;
	unsigned char *streetData = buildAlphaData(streetImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, streetData);
	delete [] streetData;*/
	
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


	backgroundBox1.pos[0] = 900;
	backgroundBox1.pos[1] = 450;
	backgroundBox1.width = 200;
	backgroundBox1.height = 100; 

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
        glBindTexture(GL_TEXTURE_2D, bguileTexture);

    glBegin(GL_QUADS);
    if (x == 3) {
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


	//draw street background box
	glColor3f(1.0,1.0,1.0);
    glPushMatrix();
    glTranslatef(backgroundBox1.pos[0], backgroundBox1.pos[1], 0);
    drawCharBox(backgroundBox1.width, backgroundBox1.height, 3);
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
				glEnd(); 
		} 
		/*Draw the play button*/ 
		glColor3f(1.0,1.0,1.0); 
		/*****/ 
		glPushMatrix(); 
		glTranslatef(w,y,1); 
		drawmenu_button(150,80); 
		glPopMatrix(); 
		/******************************************************/ 
} 

void drawmenu_button(Flt width, Flt height)
{
     int w = width, h = height;
 
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

/**************** UNDER DEVELOPMENT **********************/

void animate_ReadySetGo()
{
	
	float ReadyTimeDif = float(clock() - startTime) / CLOCKS_PER_SEC; 
	printf("%f\n", ReadyTimeDif);	
	//int w = width;
	//int h = height;
	int inc = 0;

    	
	if(ReadyTimeDif >= 0.005 && ReadyTimeDif < 0.010)
		inc = 20;
	if(ReadyTimeDif >= 0.010 && ReadyTimeDif < 0.015)
		inc = 40;
	if(ReadyTimeDif >= 0.015 && ReadyTimeDif < 0.020)
		inc = 60;
	
	if(ReadyTimeDif >= 0.020 && ReadyTimeDif < 0.025)
		inc = 80;
	if(ReadyTimeDif >= 0.025 && ReadyTimeDif < 0.030)
		inc= 100;
	if(ReadyTimeDif >= 0.030 && ReadyTimeDif < 0.035)
		inc = 120;
	if(ReadyTimeDif >= 0.035 && ReadyTimeDif < 0.040)
		inc = 140;
	if(ReadyTimeDif >= 0.040 && ReadyTimeDif < 0.045)
		inc = 160;
 	if(ReadyTimeDif >= 0.045 && ReadyTimeDif < 0.50)
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
	if(ReadyTimeDif >= 0.080 && ReadyTimeDif < 0.120){
		if(z==1){
		play_sounds=0;
		soundeffects(16);
		z++;
		}
		inc = 320;
	}


	if(ReadyTimeDif >= 0.120){
		inc = 320;
		readyPrompt = false;
		readyPromptClk = true;
		play1.control = true;
		play2.control = true;
	} 


    	glPushMatrix();
    	glTranslatef(promptBox1.pos[0] + inc, promptBox1.pos[1], 0);
    	drawCharBox(promptBox1.width, promptBox1.height, 6);
    	glPopMatrix();
	
}
/*
float ReadyTimeDif = float(clock() - startTime) / CLOCKS_PER_SEC; 
>>>>>>> b9b88d271c6cb807387e2976888719e29066d90a
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
