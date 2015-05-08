//Fight project animations
//coder: Kevin OBrien


#include "animations.h"
#include "defs.h"


extern float t;
extern bool clk, hit, finishpunch, punch1, p1control, draw;
extern Player play1, play2;
extern GLuint play1Texture;

void animatePlayerOne(Flt width, Flt height)
{
    clk =false;

    int w, h;
    float x_val;
    w = width;
    h = height;

    //printf("%f\n",t);

    if(t < 0.005)
        x_val = 0.1f;
    else if (t>=0.003 && t<.006)
        x_val = 0.2f;
    else if (t>=0.006 && t<.009)
        x_val = 0.3f;
    else if (t>=0.009 && t<.012)
        x_val = 0.4f;
    else if (t>=0.012 && t<.015){
        x_val = 0.5f;
        if (play1.pos[0] + (1.7*play1.width) >= play2.pos[0]+play2.width && t > .014)
        {
            hit = true;
            punch1 = false;
            p1control = true;
            finishpunch = true;
        }
    }
    else if (t>=0.015 && t<.018)
        x_val = 0.6f;
    else if (t>=0.018 && t<.021)
        x_val = 0.7f;
    else if (t>=0.021 && t<.024)
        x_val = 0.8f;
    else if (t>=0.024 && t<.027){
        x_val = 0.9f;
    }
    else {
        punch1 = false;
        clk = true;
        p1control = true;
        finishpunch = false;
        draw = true;
    }

    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, play1Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, 1.0f); glVertex2i(-w, -h);
    glTexCoord2f(x_val, 0.0f); glVertex2i(-w, h);
    glTexCoord2f(x_val + 0.1, 0.0f); glVertex2i(w, h);
    glTexCoord2f(x_val + 0.1, 1.0f); glVertex2i(w, -h);
    glEnd();
    glPopMatrix();
}

