// Sarahbeth Ramirez
// CS 335 - Final Project 

/****************/
/* 	CONTAINS: 

	animate readyFight - animation of "ready.. fight!" 
	init_char_boxes -	initializes boxes for title and character select screen 
	draw_char_boxes - 	draws boxes for title and character select screen
	select_box 		- 	simple use of mouse in title screen
	character_select_render 
	menu_render
	check_mouse
 */

#include <cstdio>
#include "SarahbethR.h"
#include "audio.h"
#include "defs.h"
#include "ZenaidaG.h"

Vec leftButtonPos;
int z=1, q=0, leftButtonDown = 0;
bool help_screen=false, playonce=false;
extern int xres, yres, play_sounds, selchar, title;
extern Player play1, play2;
extern clock_t startTime;
extern bool 
play_game, 
go_selchar, 
two_players, 
player1choose, 
player2choose, 
chooseBack, 
readyPrompt, 
readyPromptClk;

extern "C" {
#include "fonts.h"
}

/******* Structures ********/

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
charPrompt1box, 
charPrompt2box,
charPrompt3box,  
charPrompt4box,
promptBox1, 
promptBox2,
helpBox,
titleBox;

/******* Declare Textures ********/

Ppmimage *guileImage = NULL;
int guile = 1;
GLuint guileTexture;

Ppmimage *bguileImage = NULL;
int bguile = 1;
GLuint bguileTexture;

Ppmimage *jokerImage = NULL;
int joker = 1;
GLuint jokerTexture;

Ppmimage *redImage = NULL;
int red = 1;
GLuint redTexture;

Ppmimage *gLogoImage = NULL;
int glogo = 1;
GLuint glogoTexture;

Ppmimage *CharStreetImage = NULL;
int CharStreet = 1;
GLuint CharStreetTexture;

Ppmimage *CharForestImage = NULL;
int CharForest = 1;
GLuint CharForestTexture;

Ppmimage *charPrompt1Image = NULL;
int charPrompt1 = 1;
GLuint charPrompt1Texture;

Ppmimage *charPrompt2Image = NULL;
int charPrompt2 = 1;
GLuint charPrompt2Texture;

Ppmimage *charPrompt3Image = NULL;
int charPrompt3 = 1;
GLuint charPrompt3Texture;

Ppmimage *charPrompt4Image = NULL;
int charPrompt4 = 1;
GLuint charPrompt4Texture;

Ppmimage *promptBox1Image = NULL;
int prompt1 = 1;
GLuint promptBox1Texture;

Ppmimage *promptBox2Image = NULL;
int prompt2 = 1;
GLuint promptBox2Texture;

Ppmimage *helpImage = NULL;
int help = 1;
GLuint helpTexture;

Ppmimage *titleBoxImage = NULL;
int titlePrompt = 1;
GLuint titleBoxTexture;

Ppmimage *play1winsImage = NULL;
int play1wins = 1;
GLuint play1winsTexture;

Ppmimage *play2winsImage = NULL;
int play2wins = 1;
GLuint play2winsTexture;

extern Ppmimage *titleImage;
extern GLuint titleTexture;
extern GLuint selectTexture;

/********** Functions ***************/

void init_character_boxes(void)
{
	int w,h;

	glViewport(0, 0, xres, yres);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	/*GUILE CHARACTER BOX TEXTURE*/
	char a[] = "./images/selectGuile.ppm"; 
	guileImage = ppm6GetImage(guileImage, a);
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
	char b[] = "./images/selectBGuile.ppm";
	bguileImage = ppm6GetImage(bguileImage, b);
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
	char c[] = "./images/selectJoker.ppm"; 
	jokerImage = ppm6GetImage(jokerImage, c);
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
	char d[] = "./images/selectRed.ppm";
	redImage = ppm6GetImage(redImage, d);
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
	char e[] = "./images/charStreet.ppm";
	CharStreetImage = ppm6GetImage(CharStreetImage, e);
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
	char f[] = "./images/charForest.ppm";
	CharForestImage = ppm6GetImage(CharForestImage, f);
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
	char g[] = "./images/charPrompt1.ppm";
	charPrompt1Image = ppm6GetImage(charPrompt1Image, g);
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
	char j[] = "./images/charPrompt2.ppm";
	charPrompt2Image = ppm6GetImage(charPrompt2Image, j);
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
	char k[] = "./images/selectStage.ppm";
	charPrompt3Image = ppm6GetImage(charPrompt3Image, k);
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

	/* PROMPT TO PRESS P TO PLAY */
	char l[] = "./images/playPrompt.ppm";
	charPrompt4Image = ppm6GetImage(charPrompt4Image, l);
	glGenTextures(1, &charPrompt4Texture);
	glBindTexture(GL_TEXTURE_2D, charPrompt4Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = charPrompt4Image->width;
	h = charPrompt4Image->height;
	unsigned char *charPrompt4Data = buildAlphaData(charPrompt4Image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
					GL_RGBA, GL_UNSIGNED_BYTE, charPrompt4Data);
	delete [] charPrompt4Data;

	/* HELP SCREEN TEXTURE*/
	char m[] = "./images/helpPrompt.ppm"; 
	helpImage = ppm6GetImage(helpImage, m);
	glGenTextures(1, &helpTexture);
	glBindTexture(GL_TEXTURE_2D, helpTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = helpImage->width;
	h = helpImage->height;
	unsigned char *helpData = buildAlphaData(helpImage);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, helpData);
	delete [] helpData;

	/*READY PROMPT BOX TEXTURE*/
	char n[] = "./images/ReadyPrompt.ppm"; 
	promptBox1Image = ppm6GetImage(promptBox1Image, n);
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
	char o[] = "./images/FightPrompt.ppm"; 
	promptBox2Image = ppm6GetImage(promptBox2Image, o);
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

	//title prompt (button that says FIGHT! in title screen)
	char p[] = "./images/titlePrompt.ppm"; 
	titleBoxImage = ppm6GetImage(titleBoxImage, p);
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

	/* PLAYER 1 WINS */ 
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

	/* PLAYER 2 WINS */
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

	/********** Initialize Box Position (s) **********/

	charBox1.pos[0] = 160;
	charBox1.pos[1] = 450;
	charBox1.width = 120;
	charBox1.height = 170;

	charBox2.pos[0] = 470;
	charBox2.pos[1] = 450;
	charBox3.pos[0] = 1100;
	charBox3.pos[1] = 450;

	charBox4.pos[0] = 790;
	charBox4.pos[1] = 450;

	backgroundBox1.pos[0] = 350;
	backgroundBox1.pos[1] = 450;
	backgroundBox1.width = 250;
	backgroundBox1.height = 120; 

	backgroundBox2.pos[0] = 950;
	backgroundBox2.pos[1] = 450;

	charPrompt1box.pos[0] = (xres/2);
	charPrompt1box.pos[1] = 150;
	charPrompt1box.width = 300;
	charPrompt1box.height = 80;

	promptBox1.pos[0] = 0;
	promptBox1.pos[1] = yres/2;
	promptBox1.width = 200;
	promptBox1.height = 90;

	promptBox2.pos[0] = 0;
	promptBox2.pos[1] = yres/2;
	promptBox2.width = 300;
	promptBox2.height = 150;

	titleBox.pos[0] = 1050;
	titleBox.pos[1] = 460;
	titleBox.width = 150;
	titleBox.height = 80;

	helpBox.pos[0] = 640;
	helpBox.pos[1] = 340;
	helpBox.width = 640;
	helpBox.height = 340;
}

void selectBox(Vec leftButtonPos)
{
	if (play_game == false) { 
			if (go_selchar == false && two_players == false) {
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

	// Guile Character Box 
	if (x == 1) 
		glBindTexture(GL_TEXTURE_2D, guileTexture);

	// Bizarro Guile Character Box
	if (x == 2) 
		glBindTexture(GL_TEXTURE_2D, bguileTexture);

	// Joker Character Box 
	if (x == 10) 
		glBindTexture(GL_TEXTURE_2D, jokerTexture);

	// Bizarro Guile Character Box
	if (x ==11) 
		glBindTexture(GL_TEXTURE_2D, redTexture);

	// Select Background 1 (street)
	if (x == 3) 
		glBindTexture(GL_TEXTURE_2D, CharStreetTexture);

	// Select Background 2 (forest)
	if (x == 8) 
		glBindTexture(GL_TEXTURE_2D, CharForestTexture);

	// Select Player 1 Prompt
	if (x == 4) 
		glBindTexture(GL_TEXTURE_2D, charPrompt1Texture);

	// Select Player 2 Prompt
	if (x == 5) 
		glBindTexture(GL_TEXTURE_2D, charPrompt2Texture);

	// Select Background Prompt
	if (x == 13) 
		glBindTexture(GL_TEXTURE_2D, charPrompt3Texture);

	// Press P to Play Prompt
	if (x == 16) 
		glBindTexture(GL_TEXTURE_2D, charPrompt4Texture);

	// Help screen 
	if (x == 17) 
		glBindTexture(GL_TEXTURE_2D, helpTexture);

	/*****************************************************/
	// REQUIRES ALPHA

	// READY..... 
	if (x == 6) 
		glBindTexture(GL_TEXTURE_2D, promptBox1Texture);

	// FIGHT! 
	if (x == 7) 
		glBindTexture(GL_TEXTURE_2D, promptBox2Texture);

	// Player 1 wins
	if (x == 14) 
		glBindTexture(GL_TEXTURE_2D, play1winsTexture);

	// Player 2 wins
	if (x == 15) 
		glBindTexture(GL_TEXTURE_2D, play2winsTexture);

	// "Fight!!" on title screen
	if (x == 9)  
		glBindTexture(GL_TEXTURE_2D, titleBoxTexture);

	/********************************************************/

	if (x == 9 || x == 15 || x == 14 || x == 6 || x == 7) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER,0.1f);
		glColor4ub(255,255,255,255);
	}

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);
	}
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
}

void character_select_render(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
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
		drawCharBox(charBox1.width, charBox1.height, 2);
		glPopMatrix();

		//draw Joker character box
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charBox3.pos[0], charBox3.pos[1], 0);
		drawCharBox(charBox1.width, charBox1.height, 10);
		glPopMatrix();

		//draw Eluig character box
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charBox4.pos[0], charBox4.pos[1], 0);
		drawCharBox(charBox1.width, charBox1.height, 11);
		glPopMatrix();
	}

	// Draw Select Player Prompts

	if (two_players == false && selchar == true && player1choose == false && player2choose == false) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charPrompt1box.pos[0], charPrompt1box.pos[1], 0);
		drawCharBox(charPrompt1box.width, charPrompt1box.height, 4);
		glPopMatrix();
	} else if (two_players == false && selchar == true && player1choose == true && player2choose == false) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(charPrompt1box.pos[0], charPrompt1box.pos[1], 0);
		drawCharBox(charPrompt1box.width, charPrompt1box.height, 5);
		glPopMatrix();
	}

	if (player2choose == true) {
		// Draw street background box
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(backgroundBox1.pos[0], backgroundBox1.pos[1], 0);
		drawCharBox(backgroundBox1.width, backgroundBox1.height, 3);
		glPopMatrix();

		// Draw forest background box
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(backgroundBox2.pos[0], backgroundBox2.pos[1], 0);
		drawCharBox(backgroundBox1.width, backgroundBox1.height, 8);
		glPopMatrix();	

		// Draw "Press P to Play" Prompt 
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		if (chooseBack == false) {
			glTranslatef(charPrompt1box.pos[0], charPrompt1box.pos[1], 0);
			drawCharBox(charPrompt1box.width, charPrompt1box.height, 13);
		} else if (chooseBack == true){
			glTranslatef(charPrompt1box.pos[0], charPrompt1box.pos[1], 0);
			drawCharBox(charPrompt1box.width, charPrompt1box.height, 16);
		}
	
		glPopMatrix();	
	}

	// Enable Sound
	if (player2choose == true) {
			if (playonce == false) {
					playonce = true;
					play_sounds = 0;
					soundeffects(18);
			}
	}
}

void menu_render(void)  
{ 
	int x = 1050; 
	int y = 460; 
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
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

		// Draw FIGHT! button 
		glColor3f(1.0,1.0,1.0); 
		glPushMatrix(); 
		glTranslatef(x,y,0); 
		drawmenu_button(160,90); 
		glPopMatrix(); 

		// Draw Fonts "Click to Play"
		glEnable(GL_TEXTURE_2D);
		Rect r;
		r.center = 1;
		r.bot = 525;
		r.left = 1050;
		ggprint16(&r, 16, 0x2EFEC8, "CLICK TO PLAY!");

		r.center = 1;
		r.bot = 130;
		r.left = 430;
		ggprint16(&r, 16, 0xFF0000, "Press H for Help Menu");

		// Draw background box 
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(titleBox.pos[0], titleBox.pos[1], 0);
		drawCharBox(titleBox.width, titleBox.height, 9);
		glPopMatrix();
		
	}

	if (help_screen == true) {
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(helpBox.pos[0], helpBox.pos[1], 0);
		drawCharBox(helpBox.width, helpBox.height, 17);
		glPopMatrix();
	}
}

void drawmenu_button(Flt width, Flt height)
{
	int w = width, h = height;

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

/****************************************************/
// Setup local timers

const double oobillion1 = 1.0 / 1e9;
struct timespec timeStart1, timeCurrent1, timeMarker1;
double timeSpan1 = 0.0, countdown = 0.0;
double timeDiff1 (struct timespec *start1, struct timespec *end1) {
		return (double)(end1->tv_sec - start1->tv_sec ) +
				(double)(end1->tv_nsec - start1->tv_nsec) * oobillion1;
}
void timeCopy1(struct timespec *dest1, struct timespec *source1) {
		memcpy(dest1, source1, sizeof(struct timespec));
}
int inc = 0;

/*****************************************************/

void animate_ReadySetGo()
{
	bool incremented = false;

	float timeSpan2 = float(clock() - startTime) / CLOCKS_PER_SEC; 

	clock_gettime(CLOCK_REALTIME, &timeCurrent1); 
	timeSpan1 = timeDiff1(&timeStart1, &timeCurrent1); 

	// READY!
	if (timeSpan1 > 0.005 && incremented == false && inc <= 620) {
		inc +=20;
		timeCopy1(&timeStart1, &timeCurrent1); 
		incremented == true;
		countdown += timeSpan1;
	}
	if (timeSpan2 > 0.160 && timeSpan2 < 0.260)  
		inc = 640;

	if(timeSpan2 <= 0.260) {
		glPushMatrix();
		glTranslatef(promptBox1.pos[0] + inc, promptBox1.pos[1], 0);
		drawCharBox(promptBox1.width, promptBox1.height, 6);
		glPopMatrix();
	}

	// FIGHT!
	if (timeSpan2 >= 0.260 && timeSpan2 < 0.320) {
		inc = (xres/2);
		if (q == 1) {
			play_sounds = 0;
			soundeffects(17);
			q++;
		}
		glPushMatrix();
		glTranslatef(promptBox2.pos[0] + inc, promptBox2.pos[1], 0);
		drawCharBox(promptBox2.width, promptBox2.height, 7);
		glPopMatrix();
	}

	if (timeSpan2 >= 0.320) {
		inc = 0;
		readyPrompt = false;
		readyPromptClk = true;
		play1.control = true;
		play2.control = true;
	} 

	// Enable Sound  
	if (timeSpan2 >= 0.005 && timeSpan2 < 0.010) {
		if (q == 0) {
			play_sounds = 0;
			soundeffects(16);
			q++;
		}
	}
}

void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	if (e->type == ButtonRelease) {
		leftButtonDown = 0;
		return;
	}
		
	if (e->type == ButtonPress) {
		if (e->xbutton.button == 1) {
			leftButtonDown = 1;
			leftButtonPos[0] = (Flt)e->xbutton.x;
			leftButtonPos[1] = (Flt)(yres - e->xbutton.y);
			selectBox(leftButtonPos);
		}
	}
		
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		savex = e->xbutton.x;
		savey = e->xbutton.y;
		if (leftButtonDown) {
			leftButtonPos[0] = (Flt)e->xbutton.x;
			leftButtonPos[1] = (Flt)(yres - e->xbutton.y);
		}
	}
}


