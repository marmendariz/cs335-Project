//Fight project animations
//coder: Kevin OBrien

#include <cstdio>
#include <cstdlib>
#include "animations.h"
#include "defs.h"
#include "audio.h"

extern Player play1, play2;
extern GLuint play1guiletext;
extern GLuint play2guiletext;
extern GLuint play1bguiletext;
extern GLuint play2bguiletext;
extern GLuint play1redtext;
extern GLuint play2redtext;
extern GLuint play1jokertext;
extern GLuint play2jokertext;

extern bool player1guile,
	    player1bguile,
	    player2guile,
	    player2bguile,
	    player1red,
	    player2red,
	    player1joker,
	    player2joker;

void animatePlayerOne(Flt width, Flt height)
{
    play1.punchTimeDif = float(clock() - play1.beginPunch) / CLOCKS_PER_SEC;
    play1.punchClk =false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 0.0f;
    float y_bot = 1.0f/3.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    if(play1.punchTimeDif < 0.003)
        x_val = 0.1f;
    else if (play1.punchTimeDif>=0.003 && play1.punchTimeDif<.006)
        x_val = 0.2f;
    else if (play1.punchTimeDif>=0.006 && play1.punchTimeDif<.009)
        x_val = 0.3f;
    else if (play1.punchTimeDif>=0.009 && play1.punchTimeDif<.012){
        int rnd= (rand()%2)+5;
	soundeffects(rnd);
	x_val = 0.4f;
	}
    else if (play1.punchTimeDif>=0.012 && play1.punchTimeDif<.015){
        x_val = 0.5f;
        if (play1.pos[0] + (1.7*play1.width) >= play2.pos[0] + play2.width 
                && play1.punchTimeDif > .010 && play1.finPunch == false)
        {
            play1.punchHit = true;
            play1.punch = false;
            //play1.control = true;
            play1.finPunch = true;
        }
    }
    else if (play1.punchTimeDif>=0.015 && play1.punchTimeDif<.018)
        x_val = 0.6f;
    else if (play1.punchTimeDif>=0.018 && play1.punchTimeDif<.021)
        x_val = 0.7f;
    else if (play1.punchTimeDif>=0.021 && play1.punchTimeDif<.024)
        x_val = 0.8f;
    else if (play1.punchTimeDif>=0.024 && play1.punchTimeDif<.027){
        x_val = 0.9f;
    }
    else if (play1.punchTimeDif>=0.027){
        play1.punch = false;
        play1.punchClk = true;
        play1.control = true;
        play1.finPunch = false;

	if(play2.block==true){
       	   int rnd= (rand()%2)+7;
           soundeffects(rnd);
	}
    }

    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);

    glEnable(GL_TEXTURE_2D);


    if(player1guile==true && player1bguile==false && player1red==false && player1joker==false ){
    glBindTexture(GL_TEXTURE_2D, play1guiletext);}
    else if(player1bguile==true && player1guile==false && player1red==false && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1bguiletext);}
    else if(player1bguile==false && player1guile==false && player1red==true && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1redtext);}
    else if(player1bguile==false && player1guile==false && player1red==false && player1joker==true){
    glBindTexture(GL_TEXTURE_2D, play1jokertext);}


    

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();
}

void animatePlayerOneWalk(Flt width, Flt height){

    play1.walkTimeDif = float(clock() - play1.beginWalk)/CLOCKS_PER_SEC;
    play1.walkClk = false;
    play1.draw = false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 2.0f/3.0;
    float y_bot = 1.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    //printf("%f\n", play1.walkTimeDif);

    if(play1.walkTimeDif < 0.005)
        x_val = 0.1f;
    else if(play1.walkTimeDif >= 0.005 && play1.walkTimeDif < 0.010)
        x_val = 0.2f;
    else if(play1.walkTimeDif >= 0.010 && play1.walkTimeDif < 0.015)
        x_val = 0.3f;
    else if(play1.walkTimeDif >= 0.015 && play1.walkTimeDif < 0.020)
        x_val = 0.4f;
    else if(play1.walkTimeDif >= 0.020 && play1.walkTimeDif < 0.025)
        x_val = 0.5f;


    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);

    glEnable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D, play1Texture);
    //if(player1guile==true && player1bguile==false){
   // glBindTexture(GL_TEXTURE_2D, play1guiletext);}
  //  else if(player1bguile==true && player1guile==false){
     if(player1guile==true && player1bguile==false && player1red==false && player1joker==false ){
    glBindTexture(GL_TEXTURE_2D, play1guiletext);}
    else if(player1bguile==true && player1guile==false && player1red==false && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1bguiletext);}
    else if(player1bguile==false && player1guile==false && player1red==true && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1redtext);}
    else if(player1bguile==false && player1guile==false && player1red==false && player1joker==true){
    glBindTexture(GL_TEXTURE_2D, play1jokertext);}

  //glBindTexture(GL_TEXTURE_2D, play1bguiletext);}
 
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();

    if(play1.walkTimeDif >= 0.025){
        play1.walkClk = true;
        play1.draw = true;
    }
}

void animatePlayerOneBlock(Flt width, Flt height){

    play1.blockTimeDif = float(clock() - play1.beginBlock)/CLOCKS_PER_SEC;
    play1.blockClk = false;
    play1.draw = false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 1.0f/3.0;
    float y_bot = 2.0f/3.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    //printf("%f\n", play1.walkTimeDif);

    if(play1.blockTimeDif < 0.005){
        x_val = 0.1f;
	}
    else if(play1.blockTimeDif >= 0.005)
        x_val = 0.2f;

    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);

    glEnable(GL_TEXTURE_2D);
    //if(player1guile==true && player1bguile==false){
   // glBindTexture(GL_TEXTURE_2D, play1guiletext);}
   // else if(player1bguile==true && player1guile==false){
   // glBindTexture(GL_TEXTURE_2D, play1bguiletext);}
    if(player1guile==true && player1bguile==false && player1red==false && player1joker==false ){
    glBindTexture(GL_TEXTURE_2D, play1guiletext);}
    else if(player1bguile==true && player1guile==false && player1red==false && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1bguiletext);}
    else if(player1bguile==false && player1guile==false && player1red==true && player1joker==false){
    glBindTexture(GL_TEXTURE_2D, play1redtext);}
    else if(player1bguile==false && player1guile==false && player1red==false && player1joker==true){
    glBindTexture(GL_TEXTURE_2D, play1jokertext);}


    //glBindTexture(GL_TEXTURE_2D, play1Texture);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();

}


void animatePlayerTwo(Flt width, Flt height)
{
    play2.punchTimeDif = float(clock() - play2.beginPunch) / CLOCKS_PER_SEC;
    play2.punchClk =false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 0.0f;
    float y_bot = 1.0f/3.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    if(play2.punchTimeDif < 0.003)
        x_val = 0.9f;
    else if (play2.punchTimeDif>=0.003 && play2.punchTimeDif<.006)
        x_val = 0.8f;
    else if (play2.punchTimeDif>=0.006 && play2.punchTimeDif<.009)
        x_val = 0.7f;
    else if (play2.punchTimeDif>=0.009 && play2.punchTimeDif<.012){
        int rnd=(rand()%2)+5;
        soundeffects(rnd);
	x_val = 0.6f;
	}
    else if (play2.punchTimeDif>=0.012 && play2.punchTimeDif<.015){
        x_val = 0.5f;
        if (play1.pos[0] + (1.7*play1.width) >= play2.pos[0] + play2.width 
                && play2.punchTimeDif > .010 && play2.finPunch == false)
        {
            play2.punchHit = true;
            play2.punch = false;
            //play2.control = true;
            play2.finPunch = true;
        }
    }
    else if (play2.punchTimeDif>=0.015 && play2.punchTimeDif<.018)
        x_val = 0.4f;
    else if (play2.punchTimeDif>=0.018 && play2.punchTimeDif<.021)
        x_val = 0.3f;
    else if (play2.punchTimeDif>=0.021 && play2.punchTimeDif<.024)
        x_val = 0.2f;
    else if (play2.punchTimeDif>=0.024 && play2.punchTimeDif<.027){
        x_val = 0.1f;
    }
    else if (play2.punchTimeDif>=0.027){
        play2.punch = false;
        play2.punchClk = true;
        play2.control = true;
        play2.finPunch = false;
	if(play1.block==true){
       	   int rnd= (rand()%2)+7;
           soundeffects(rnd);
	}
        //play2.draw = true;
    }

    glPushMatrix();
    glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);

    glEnable(GL_TEXTURE_2D);

 //   if(player2guile==true && player2bguile==false){
  //  glBindTexture(GL_TEXTURE_2D, play2guiletext);}
    //else if(player2bguile==true && player2guile==false){
    //glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
  // glBindTexture(GL_TEXTURE_2D, play2Texture);
   if(player2guile==true && player2bguile==false && player2red==false && player2joker==false ){
    glBindTexture(GL_TEXTURE_2D, play2guiletext);}
    else if(player2bguile==true && player2guile==false && player2red==false && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
    else if(player2bguile==false && player2guile==false && player2red==true && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2redtext);}
    else if(player2bguile==false && player2guile==false && player2red==false && player2joker==true){
    glBindTexture(GL_TEXTURE_2D, play2jokertext);}


    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();
}

void animatePlayerTwoWalk(Flt width, Flt height){

    play2.walkTimeDif = float(clock() - play2.beginWalk)/CLOCKS_PER_SEC;
    play2.walkClk = false;
    play2.draw = false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 2.0f/3.0;
    float y_bot = 1.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    //printf("%f\n", play1.walkTimeDif);

    if(play2.walkTimeDif < 0.005)
        x_val = 0.8f;
    else if(play2.walkTimeDif >= 0.005 && play2.walkTimeDif < 0.010)
        x_val = 0.7f;
    else if(play2.walkTimeDif >= 0.010 && play2.walkTimeDif < 0.015)
        x_val = 0.6f;
    else if(play2.walkTimeDif >= 0.015 && play2.walkTimeDif < 0.020)
        x_val = 0.5f;
    else if(play2.walkTimeDif >= 0.020 && play2.walkTimeDif < 0.025)
        x_val = 0.4f;
    else{
        play2.walkClk = true;
        play2.draw = true;
    }

    glPushMatrix();
    glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);

    glEnable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D, play2Texture);
   // if(player2guile==true && player2bguile==false){
   // glBindTexture(GL_TEXTURE_2D, play2guiletext);}
    //else if(player2bguile==true && player2guile==false){
    //glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
	if(player2guile==true && player2bguile==false && player2red==false && player2joker==false ){
    glBindTexture(GL_TEXTURE_2D, play2guiletext);}
    else if(player2bguile==true && player2guile==false && player2red==false && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
    else if(player2bguile==false && player2guile==false && player2red==true && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2redtext);}
    else if(player2bguile==false && player2guile==false && player2red==false && player2joker==true){
    glBindTexture(GL_TEXTURE_2D, play2jokertext);}

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();


}

void animatePlayerTwoBlock(Flt width, Flt height){

    play2.blockTimeDif = float(clock() - play2.beginBlock)/CLOCKS_PER_SEC;
    play2.blockClk = false;
    play2.draw = false;

    int w, h;
    float x_val = 0.0f;
    float y_top = 1.0f/3.0;
    float y_bot = 2.0f/3.0f;
    float inc = 0.1f;
    w = width;
    h = height;

    //printf("%f\n", play1.walkTimeDif);

    if(play2.blockTimeDif < 0.005){
        x_val = 0.8f;
	}
    else if(play2.blockTimeDif >= 0.005)
        x_val = 0.7f;

    glPushMatrix();
    glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);

    glEnable(GL_TEXTURE_2D);
//    if(player2guile==true && player2bguile==false){
//    glBindTexture(GL_TEXTURE_2D, play2guiletext);}
//    else if(player2bguile==true && player2guile==false){
//   glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
 if(player2guile==true && player2bguile==false && player2red==false && player2joker==false ){
    glBindTexture(GL_TEXTURE_2D, play2guiletext);}
    else if(player2bguile==true && player2guile==false && player2red==false && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2bguiletext);}
    else if(player2bguile==false && player2guile==false && player2red==true && player2joker==false){
    glBindTexture(GL_TEXTURE_2D, play2redtext);}
    else if(player2bguile==false && player2guile==false && player2red==false && player2joker==true){
    glBindTexture(GL_TEXTURE_2D, play2jokertext);}

    //glBindTexture(GL_TEXTURE_2D, play1Texture);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, y_bot); glVertex2i(-w, -h);
    glTexCoord2f(x_val, y_top); glVertex2i(-w, h);
    glTexCoord2f(x_val + inc, y_top); glVertex2i(w, h);
    glTexCoord2f(x_val + inc, y_bot); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();

}
