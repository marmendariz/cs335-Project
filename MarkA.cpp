//Initialize textures
//Mark Armendariz
#include "MarkA.h"
#include "defs.h"
#include "ppm.h"
#include <stdio.h>
extern "C" {
#include "fonts.h"
}

extern Ppmimage *play2guile,
       *play1guile,
       *play2bguile,
       *play1bguile,
       *play1red,
       *play2red,
       *play1joker,
       *play2joker,
       *metalImage,
       *metal2Image,
       *streetImage,
       *forestImage;
extern GLuint play1guiletext,
       play2guiletext,
       play1bguiletext,
       play2bguiletext,
       play1redtext,
       play2redtext,
       play1jokertext,
       play2jokertext,
       metalTexture,
       metal2Texture,
       streetTexture,
       forestTexture;
extern Flt metal2Pos[4],
       metal2Dim[4];  
extern int forest,
       xres,
       yres;
extern bool player1choose,
       player2choose,
       player1guile,
       player2guile,
       player1bguile,
       player2bguile,
       player1red,
       player2red,
       player1joker,
       player2joker,
       two_players; 
extern int  player1,
       player2;


extern int please;
void init_opengl(void)
{
	int w,h;
	/*Initialize openGL and all textures*/
	glViewport(0, 0, xres, yres);
	/*Initialize matrices*/
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	/*This sets 2D mode (no perspective)*/
	glOrtho(0, xres, 0, yres, -1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	/*Do this to allow fonts*/
	glEnable(GL_TEXTURE_2D);
        initialize_fonts();


         printf("initialize textures\n");
	/**************INITIALIZE TEXTURES******************/
	/*PLAYER ONE TEXTURE*/
	//char player1char[] = "./images/guile_play1.ppm";
	//play1Image = ppm6GetImage(play1Image,player1char);
	//if(player1guile==true && player1bguile==false){
		char player1guile[]= "./images/guile_play1.ppm";
		play1guile = ppm6GetImage(play1guile,player1guile);
		glGenTextures(1, &play1guiletext);
		glBindTexture(GL_TEXTURE_2D, play1guiletext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1guile->width;
		 h = play1guile->height;
		unsigned char *play1Dataguile = buildAlphaData(play1guile);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Dataguile);
	        printf("guile\n");
	//delete [] play1Data;
	//}
 	// if(player1guile==false && player1bguile==true){
		char player1bguile[]="./images/bizguile_play1.ppm";
		play1bguile = ppm6GetImage(play1bguile,player1bguile);
		glGenTextures(1, &play1bguiletext);
		glBindTexture(GL_TEXTURE_2D, play1bguiletext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1bguile->width;
		 h = play1bguile->height;
		unsigned char *play1Databguile = buildAlphaData(play1bguile);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Databguile);
	        printf("guile\n");
		//delete [] play1Data;
	//}
       		char player1red[]="./images/red_play1.ppm";
		play1red = ppm6GetImage(play1red,player1red);
		glGenTextures(1, &play1redtext);
		glBindTexture(GL_TEXTURE_2D, play1redtext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1red->width;
		 h = play1red->height;
		unsigned char *play1Datared = buildAlphaData(play1red);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Datared);
	        printf("red guile\n");

		char player1joker[]="./images/joker_play1.ppm";
		play1joker = ppm6GetImage(play1joker,player1joker);
		glGenTextures(1, &play1jokertext);
		glBindTexture(GL_TEXTURE_2D, play1jokertext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1joker->width;
		 h = play1joker->height;
		unsigned char *play1Datajoker = buildAlphaData(play1joker);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Datajoker);
	        printf("joker guile\n");



	/*PLAYER TWO TEXTURE*/
	//char player2char[] = "./images/guile_play2.ppm";
	// if(player2guile==true && player2bguile==false){
		char player2guile[]="./images/guile_play2.ppm";
		play2guile = ppm6GetImage(play2guile,player2guile);
		glGenTextures(1, &play2guiletext);
		glBindTexture(GL_TEXTURE_2D, play2guiletext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2guile->width;
		 h = play2guile->height;
		unsigned char *play2Dataguile = buildAlphaData(play2guile);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,GL_RGBA, GL_UNSIGNED_BYTE, play2Dataguile);
	        printf("bguile\n");
		//delete [] play2Data;
	//}
	// if(player2guile==false && player2bguile==true){
		char player2bguile[]="./images/bizguile_play2.ppm";
		play2bguile = ppm6GetImage(play2bguile,player2bguile);
		glGenTextures(1, &play2bguiletext);
		glBindTexture(GL_TEXTURE_2D, play2bguiletext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2bguile->width;
		 h = play2bguile->height;
		unsigned char *play2Databguile = buildAlphaData(play2bguile);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play2Databguile);
	        printf("bguile\n");
		//delete [] play2Data;
	//}
                char player2red[]="./images/red_play2.ppm";
		play2red = ppm6GetImage(play2red,player2red);
		glGenTextures(1, &play2redtext);
		glBindTexture(GL_TEXTURE_2D, play2redtext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2red->width;
		 h = play2red->height;
		unsigned char *play2Datared = buildAlphaData(play2red);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play2Datared);
	        printf("red guile\n");

     		char player2joker[]="./images/joker_play2.ppm";
		play2joker = ppm6GetImage(play2joker,player2joker);
		glGenTextures(1, &play2jokertext);
		glBindTexture(GL_TEXTURE_2D, play2jokertext);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2joker->width;
		 h = play2joker->height;
		unsigned char *play2Datajoker = buildAlphaData(play2joker);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play2Datajoker);
	        printf("joker guile\n");

	
	/*BACKGROUND METAL HEALTHBAR*/
	char y[] = "./images/metal.ppm";
	metalImage = ppm6GetImage(metalImage,y);
	glGenTextures(1, &metalTexture);
	glBindTexture(GL_TEXTURE_2D, metalTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	 w = metalImage->width;
	 h = metalImage->height;
	unsigned char *metalData = buildAlphaData(metalImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, metalData);
	//delete [] metalData;

	/*TOP LAYER METAL HEALTHBAR*/
	char c[] = "./images/metal3.ppm";
	metal2Image = ppm6GetImage(metal2Image,c);
	glGenTextures(1, &metal2Texture);
	glBindTexture(GL_TEXTURE_2D, metal2Texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	w = metal2Image->width;
	h = metal2Image->height;
	unsigned char *metal2Data = buildAlphaData(metal2Image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, metal2Data);
	//delete [] metal2Data;

	/*FOREST BACKGROUND IMAGE*/
	char z[] = "./images/forest.ppm";
	forestImage = ppm6GetImage(forestImage,z);
	glGenTextures(1, &forestTexture);
	glBindTexture(GL_TEXTURE_2D, forestTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			forestImage->width, forestImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);
 
	char u[] = "./images/street.ppm";
        streetImage = ppm6GetImage(streetImage, u);
	glGenTextures(1, &streetTexture);
	glBindTexture(GL_TEXTURE_2D, streetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	w = streetImage->width;
	h = streetImage->height;
	unsigned char *streetData = buildAlphaData(streetImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, streetData);
	delete [] streetData;
	/****************END INITIALIZE TEXTURES*********************/

}


unsigned char *buildAlphaData(Ppmimage *img)
{
	/*add 4th component to RGB stream*/
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = new unsigned char[img->width * img->height * 4];
	ptr = newdata;
	for (int i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}
