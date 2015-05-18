//Initialize textures
//Mark Armendariz
#include "MarkA.h"
#include "defs.h"
#include "ppm.h"
extern "C" {
#include "fonts.h"
}

extern Ppmimage *play2Image,
       *play1Image,
       *metalImage,
       *metal2Image,
       *forestImage;
extern GLuint play1Texture,
       play2Texture,
       metalTexture,
       metal2Texture,
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
       two_players; 
extern int  player1,
       player2;



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


	/**************INITIALIZE TEXTURES******************/
	/*PLAYER ONE TEXTURE*/
	//char player1char[] = "./images/guile_play1.ppm";
	//play1Image = ppm6GetImage(play1Image,player1char);
	if(player1guile==true && player1bguile==false){
		char player1char[]= "./images/guile_play1.ppm";
		play1Image = ppm6GetImage(play1Image,player1char);
		glGenTextures(1, &play1Texture);
		glBindTexture(GL_TEXTURE_2D, play1Texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1Image->width;
		 h = play1Image->height;
		unsigned char *play1Data = buildAlphaData(play1Image);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Data);
		//delete [] play1Data;
	}
 	 if(player1guile==false && player1bguile==true){
		char player1char[]="./images/bizguile_play1.ppm";
		play1Image = ppm6GetImage(play1Image,player1char);
		glGenTextures(1, &play1Texture);
		glBindTexture(GL_TEXTURE_2D, play1Texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play1Image->width;
		 h = play1Image->height;
		unsigned char *play1Data = buildAlphaData(play1Image);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play1Data);
		//delete [] play1Data;
	}


	/*PLAYER TWO TEXTURE*/
	//char player2char[] = "./images/guile_play2.ppm";
	 if(player2guile==true && player2bguile==false){
		char player2char[]="./images/guile_play2.ppm";
		play2Image = ppm6GetImage(play2Image,player2char);
		glGenTextures(1, &play2Texture);
		glBindTexture(GL_TEXTURE_2D, play2Texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2Image->width;
		 h = play2Image->height;
		unsigned char *play2Data = buildAlphaData(play2Image);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,GL_RGBA, GL_UNSIGNED_BYTE, play2Data);
	}
	 if(player2guile==false && player2bguile==true){
		char player2char[]="./images/bizguile_play2.ppm";
		play2Image = ppm6GetImage(play2Image,player2char);
		glGenTextures(1, &play2Texture);
		glBindTexture(GL_TEXTURE_2D, play2Texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		 w = play2Image->width;
		 h = play2Image->height;
		unsigned char *play2Data = buildAlphaData(play2Image);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, play2Data);
	}


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
	delete [] metalData;

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
	delete [] metal2Data;

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
//
