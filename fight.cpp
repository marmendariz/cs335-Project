
/*ULTIMATE PUNCH FRENZY:
  IMMORTAL PEACETIME:
  FIGHTERS IN THE STREET

  cs335 - Software Engineering Project
  Team #6
  Mark
  Zenaida
  Sarahbeth
  Kevin
 */
/**********************************************/
/**********************************************/
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
extern "C" {
#include "fonts.h"
}
bool title_flag = true; 
typedef float Flt;
typedef Flt Vec[3];
#define MakeVector(x,y,z,v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a) (a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];

//X Windows global variables
Display *dpy;
Window win;
GLXContext glc;

/*KEY ARRAY*/
int keys[65536];

void initXWindows(void);
void init_opengl(void);
unsigned char *buildAlphaData(Ppmimage *img);
void init_players(void);
void init_healthBars(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);
void animatePlayerOne(Flt, Flt);

int xres=1280, yres=680;
int leftButtonDown=0;
int punchDamage = 5;
Vec leftButtonPos;

bool punch1 = false;
bool punch2 = false;

typedef struct t_healthBar
{
    Vec pos;
    Vec posOut;
    float width, height;
    float wOut, hOut;
} healthBar;

typedef struct t_attacks
{
    Vec posPunch;
    float wPunch, hPunch;
}Attacks;

typedef struct t_Player {
    Vec pos;
    Vec vel;
    Vec center;
    float radius;
    float mass;
    float width,height;
    healthBar hbar;
    Attacks atk;
} Player;
Player play1;
Player play2;

/*TEXTURE STUFF*/
Ppmimage *play1Image=NULL;
GLuint play1Texture;

Ppmimage *metalImage=NULL;
GLuint metalTexture;

Ppmimage *metal2Image=NULL;
GLuint metal2Texture;
Vec pos;
Vec dim; 

Ppmimage *forestImage=NULL;
GLuint forestTexture;
int forest =1;

Ppmimage *titleImage=NULL;
int title =1;
GLuint titleTexture;

/*****/
clock_t begin_time;
bool clk = true;
float t;
bool hit = false;
/*****/
//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
	(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

int main(void)
{
    initXWindows();
    init_players();
    init_opengl();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    int done=0;
    while(!done) {
	while(XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_resize(&e);
	    check_mouse(&e);
	    done = check_keys(&e);
	}
	clock_gettime(CLOCK_REALTIME, &timeCurrent);
	timeSpan = timeDiff(&timeStart, &timeCurrent);
	timeCopy(&timeStart, &timeCurrent);
	physicsCountdown += timeSpan;
	while(physicsCountdown >= physicsRate) {
	    physics();
	    physicsCountdown -= physicsRate;
	}
	render();
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
    return 0;
}

void cleanupXWindows(void)
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Ultimate Punch Frenzy: Immortal Peacetime: Fighters in the Street");
}

void setup_screen_res(const int w, const int h)
{
    xres = w;
    yres = h;
}

void initXWindows(void)
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XSetWindowAttributes swa;

    setup_screen_res(1280, 680);
    dpy = XOpenDisplay(NULL);
    if(dpy == NULL) {
	std::cout << "\n\tcannot connect to X server\n" << std::endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
	std::cout << "\n\tno appropriate visual found\n" << std::endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask =
	ExposureMask |
	KeyPressMask |
	KeyReleaseMask |
	PointerMotionMask |
	ButtonPressMask |
	ButtonReleaseMask |
	StructureNotifyMask |
	SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
	    vi->depth, InputOutput, vi->visual,
	    CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
    //window has been resized.
    setup_screen_res(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, xres, 0, yres, -1, 1);
    set_title();
}

/*I changed this function!!*/
void init_opengl(void)
{
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

    /*************************************************************/	
		char x[] = "./images/title.ppm";
	titleImage = ppm6GetImage(titleImage,x);
	
    //create opengl texture elements
    glGenTextures(1, &titleTexture);
    glBindTexture(GL_TEXTURE_2D, titleTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                            titleImage->width, titleImage->height,
                            0, GL_RGB, GL_UNSIGNED_BYTE, titleImage->data);
   	
    /*******************************************/
   	if (title_flag) { 
		/*load the images file into a ppm structure*/
    	 char x[] = "./images/sprite1.ppm";
    	play1Image = ppm6GetImage(play1Image,x);

    	/*Create opengl texture elements*/
    	glGenTextures(1, &play1Texture);
    	glBindTexture(GL_TEXTURE_2D, play1Texture);

    	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    	int w = play1Image->width;
    	int h = play1Image->height;
    	//must build a new set of data...
    	unsigned char *play1Data = buildAlphaData(play1Image);	
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, play1Data);
    	delete [] play1Data;
    /*****************************************/

    	char y[] = "./images/metal.ppm";
    	//play1Image = ppm6GetImage("./images/anim-1.ppm");
    	//
    	metalImage = ppm6GetImage(metalImage,y);
    	//create opengl texture elements
    	glGenTextures(1, &metalTexture);

    	glBindTexture(GL_TEXTURE_2D, metalTexture);
    	//
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    	w = metalImage->width;
    	h = metalImage->height;
    	//must build a new set of data...
    	unsigned char *metalData = buildAlphaData(metalImage);	
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, metalData);
	    delete [] metalData;

   
	/******************************************************************/
    	char z[] = "./images/forest.ppm";

    	forestImage = ppm6GetImage(forestImage,z);

    	//create opengl texture elements
    	glGenTextures(1, &forestTexture);
    	glBindTexture(GL_TEXTURE_2D, forestTexture);
    	//
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    	glTexImage2D(GL_TEXTURE_2D, 0, 3,
                            forestImage->width, forestImage->height,
                            0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);
    
	/****************************************************************/
	}
}

unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
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


void init_players(void)
{
    /*Player ONE*/
    play1.pos[0] = 350;
    play1.pos[1] = 200;
    play1.vel[0] = 0.0;
    play1.vel[1] = 0.0;
    play1.width = 120.0;
    play1.height= 120.0;
    play1.radius= 30.0;
    play1.mass = 1.0;

    play1.atk.wPunch = 100;
    play1.atk.hPunch = 10;

    /*Player TWO*/
    play2.pos[0] = xres - 350;
    play2.pos[1] = 200;
    play2.vel[0] = 0.0;
    play2.vel[1] = 0.0;
    play2.radius = 30.0;
    play2.width = 120.0;
    play2.height= 120.0;
    play2.mass = 1.0;

    init_healthBars();
}

void init_healthBars()
{
    play1.hbar.pos[0] = 250;
    play1.hbar.pos[1] = 620;
    play1.hbar.width = 235;
    play1.hbar.height = 20;

    play1.hbar.posOut[0] = 250;
    play1.hbar.posOut[1] = 620;
    play1.hbar.wOut = 240;
    play1.hbar.hOut =25;

    play2.hbar.pos[0] = xres - 250;
    play2.hbar.pos[1] = 620;
    play2.hbar.width = 235;
    play2.hbar.height = 20;

    play2.hbar.posOut[0] = xres - 250;
    play2.hbar.posOut[1] = 620;
    play2.hbar.wOut = 240;
    play2.hbar.hOut = 25;

}

void check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != xres || xce.height != yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
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


int check_keys(XEvent *e)
{
    int key= XLookupKeysym(&e->xkey, 0);

    if (e->type == KeyRelease)
    {
	keys[key]=0;
	return 0;
    }
    if (e->type == KeyPress)
	keys[key]=1;
    else
	return 0;

    switch(key) 
    {
	case XK_Escape:
	    return 1;
	    break;
    case XK_b:
        forest ^=1;
        break;
	case XK_p:
		title ^=1;
		break;
	    /*
	       case XK_Left:
	       play1.vel[0] -= 1.0;
	       break;
	       case XK_a:
	       play2.vel[0] -= 1.0;
	       break;
	       case XK_s:
	       play2.vel[1] -= 1.0;
	       break;
	       case XK_d:
	       play2.vel[0] += 1.0;
	       break;
	       case XK_w:
	       play2.vel[1] += 1.0;
	       break;
	       case XK_Right:
	       play1.vel[0] += 1.0;
	       break;
	       case XK_Up:
	       play1.vel[1] += 1.0;
	       break;
	       case XK_Down:
	       play1.vel[1] -= 1.0;
	       break;
	       case XK_x:
	    //press s to slow the balls
	    play1.vel[0] *= 0;
	    play1.vel[1] *= 0;
	    play2.vel[0] *= 0;
	    play2.vel[1] *= 0;
	    break;*/
    }
    return 0;
}

void VecNormalize(Vec v)
{
    Flt dist = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
    if (dist==0.0)
	return;
    Flt len = 1.0 / sqrt(dist);
    v[0] *= len;
    v[1] *= len;
    v[2] *= len;
}


void physics(void)
{
   //check for collision here
    Flt distance, speed;
    Flt tx;
        //Flt ty;
    Flt movi[2], movj[2];
    Flt massFactor, massi, massj;
    Vec vcontact[2];
    Vec vmove[2];
    //Flt dot0,dot1;
    tx = play1.pos[0] - play2.pos[0];
    distance = sqrt(tx * tx);
    double stepVel = 4.0;

    /*CHECK KEYS*/
    /*Player ONE*/
    if(keys[XK_a])
        play1.pos[0] -= stepVel;    
    if(keys[XK_d])
        play1.pos[0] += stepVel;

    /*Player TWO*/
    if(keys[XK_Left])
        play2.pos[0] -= stepVel;
    if(keys[XK_Right])
        play2.pos[0] += stepVel;
    /*END CHECK KEYS*/

    play1.atk.posPunch[0] = play1.pos[0]; 
    play1.atk.posPunch[1] = play1.pos[1] + play1.height - 100;
    

    if (distance < play1.width+(play2.width)) {
        //We have a collision!
        //vector from center to center.
        vcontact[0][0] = tx;
        //vcontact[0][1] = ty;
        vcontact[0][2] = 0.0;
        VecNormalize(vcontact[0]);
        VecCopy(vcontact[0],vcontact[1]);
        VecNegate(vcontact[1]);
        movi[0] = play1.vel[0];
        movi[1] = play1.vel[1];
        movj[0] = play2.vel[0];
        movj[1] = play2.vel[1];
        vmove[0][0] = movi[0];
        //vmove[0][1] = movi[1];
        vmove[0][2] = 0.0;
        vmove[1][0] = movj[0];
        //vmove[1][1] = movj[1];
        vmove[1][2] = 0.0;
        VecNormalize(vmove[0]);
        VecNormalize(vmove[1]);
        //Determine how direct the hit was...
        //dot0 = VecDot(vcontact[0], vmove[0]);  //dot product
        //dot1 = VecDot(vcontact[1], vmove[1]);  //dot product
        //Find the closing (relative) speed of the objects...
        float speed0 = 0.5;
                //sqrtf( movi[0]*movi[0] + movi[1]*movi[1]) * dot0;
        float speed1 = 0.5;
                //sqrtf( movj[0]*movj[0] + movj[1]*movj[1]) * dot1;
        speed = speed0 + speed1;
        if (speed < 0.0) {
            //Normalize the mass of each object...
            massFactor = 2.0 / (play1.mass + play2.mass);
            massi = play1.mass * massFactor;
            massj = play2.mass * massFactor;
            play2.vel[0] += vcontact[0][0] * speed * massi;
            play2.vel[1] += vcontact[0][1] * speed * massi;
            play1.vel[0] += vcontact[1][0] * speed * massj;
            play1.vel[1] += vcontact[1][1] * speed * massj;
        }
    }

    //Update position
    while(play1.pos[0] >= play2.pos[0])
    {
        play1.pos[0]-= 2;
        play2.pos[0]+= 2;
    }
    if(play1.pos[0]>(30 + play2.pos[0]))
    {
        play1.pos[0] += play1.vel[0];
        play2.pos[0] += play2.vel[0];
    }

    //steps
    play1.vel[0] = 0;
    play2.vel[0] = 0;

    
    while(play1.pos[0] - play1.width  < 0){
    play1.pos[0] += 2;
    }

    while(play2.pos[0] + play2.width > xres) {
    play2.pos[0] -= 2;
    }
     
    //}

    //Update position
    while(play1.pos[0]+play1.width>=play2.pos[0])
    {
	play1.pos[0]-= 2;
	play2.pos[0]+= 2;
    }
    if(play1.pos[0]<(30+play2.pos[0]))
    {
	play1.pos[0] += play1.vel[0];
	play2.pos[0] += play2.vel[0];
    }

    //steps
    play1.vel[0] = 0;
    play2.vel[0] = 0;

    //Check for collision with window edges
    if ((play1.pos[0] < play1.width && play1.vel[0] < 0.0) ||
	    (play1.pos[0] >= (Flt)xres-play1.width && play1.vel[0] > 0.0)) {
	play1.vel[0] = -play1.vel[0];
    }
    if ((play1.pos[1] < play1.width && play1.vel[1] < 0.0) ||
	    (play1.pos[1] >= (Flt)yres-play1.width && play1.vel[1] > 0.0)) {
	play1.vel[1] = -play1.vel[1];
    }
    if ((play2.pos[0] < play2.radius && play2.vel[0] < 0.0) ||
	    (play2.pos[0] >= (Flt)xres-play2.radius && play2.vel[0] > 0.0)) {
	play2.vel[0] = -play2.vel[0];
    }
    if ((play2.pos[1] < play2.radius && play2.vel[1] < 0.0) ||
	    (play2.pos[1] >= (Flt)yres-play2.radius && play2.vel[1] > 0.0)) {
	play2.vel[1] = -play2.vel[1];
    }
}

void drawBox(Flt width, Flt height, int x)
{
    int w = width;
    int h = height;

    /*If x==1, setup player texture setting*/
    if(x==1)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, play1Texture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0.1f);
        glColor4ub(255,255,255,255);
    }

    /*If x==0, setup metal texture for healthbar*/
    if(x==0)
        glBindTexture(GL_TEXTURE_2D, metalTexture);

    /*If x==2, Draw red bars - No texture*/
    if(x==2)
        glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    if(x==1)
    {
    glTexCoord2f(0.0f, 0.5f); glVertex2i(-w, -h);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
    glTexCoord2f(0.2f, 0.0f); glVertex2i(w, h);
    glTexCoord2f(0.2f, 0.5f); glVertex2i(w, -h);    
    }
    if(x==0)
    {
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);	
    }
    else /*Draw non-textured rectangle*/
    {
        glVertex2i(-w, -h);
        glVertex2i(-w, h);
        glVertex2i(w, h);
        glVertex2i(w, -h);     
    }
    glEnd();

} 
void render(void)
{
    Rect r;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /***************************************/
	/* Draw title screen background */

	glColor3f(1.0, 1.0, 1.0);
    if (title) {
        glBindTexture(GL_TEXTURE_2D, titleTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
    } else {

	/******************************************************/
    glColor3f(1.0, 1.0, 1.0);
    if (forest) {
        glBindTexture(GL_TEXTURE_2D, forestTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
        glEnd();
    }

    /*************************************/
    /*Draw Player One*/
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);
    if(!punch1)
    	drawBox(play1.width,play1.height,1);
    glPopMatrix();
    /*Draw Player Two*/
    glPushMatrix();
    glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);
    drawBox(play2.width,play2.height,1);
    glPopMatrix();
    /***************************************/


    /***************DRAW HEALTHBARS*******************/
    /*************************************************/
    /*Draw Frames first*/
    glDisable(GL_ALPHA_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(play1.hbar.posOut[0], 
	    play1.hbar.posOut[1], play1.hbar.posOut[2]);
    drawBox(play1.hbar.wOut,play1.hbar.hOut,0);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(play2.hbar.posOut[0], 
	    play2.hbar.posOut[1], play2.hbar.posOut[2]);
    drawBox(play2.hbar.wOut,play2.hbar.hOut,0);
    glPopMatrix();
    /****/

    /****/
    /*Draw inner-red bar*/
    glColor3ub(200,0,0);
    glPushMatrix();
    glTranslatef(play1.hbar.pos[0], 
	    play1.hbar.pos[1], play1.hbar.pos[2]);
    drawBox(play1.hbar.width,play1.hbar.height,2);
    glPopMatrix();

    glColor3ub(200,0,0);
    glPushMatrix();
    glTranslatef(play2.hbar.pos[0], 
	    play2.hbar.pos[1], play2.hbar.pos[2]);
    drawBox(play2.hbar.width,play2.hbar.height,2);
    glPopMatrix();
    /*****************END DRAW HEALTHBARS***********************/

    /*Draw Rectable for temporary punch*/
    if(keys[XK_f]){
        punch1 = true;
        /*
	glColor3ub(200,0,0);
	glPushMatrix();
	glTranslatef(play1.atk.posPunch[0], 
		play1.atk.posPunch[1], play1.atk.posPunch[2]);
	drawBox(play1.atk.wPunch,play1.atk.hPunch,0);
	glPopMatrix();
    */
    
	
    }

    t = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
    if(punch1 && !hit)
    {
        if(clk)
        {
            begin_time = clock();

        }
        animatePlayerOne(play1.width,play1.height);
    }

    if(!punch1 && hit){
	play2.pos[0] += 100;
	play2.hbar.pos[0] += punchDamage;
	play2.hbar.width -= punchDamage;
	hit = false;
    }



if(play2.hbar.width <= 0){
	play2.hbar.width = 0;
	punchDamage = 0;
    }
    glEnable(GL_TEXTURE_2D);
    r.bot = yres - 20;
    r.center = 5;
    r.left = play1.hbar.posOut[0]-180;
    r.bot  = play1.hbar.posOut[1]-10;
    ggprint16(&r, 20, 0x00ffff00, "Player One");
    r.left = play2.hbar.posOut[0]+180;
    r.bot  = play2.hbar.posOut[1]-10;
    ggprint16(&r, 20, 0x00ffff00, "Player Two");
}
}


void animatePlayerOne(Flt width, Flt height)
{
    clk =false;

    int w, h;
    float x_val;
    w = width;
    h = height;

    printf("%f\n",t);

    if(t < 0.005)
	x_val = 0.2f;
    else if (t>=0.005 && t<.01)
	x_val = 0.4f;
    else if (t>=0.01 && t<.015)
	x_val = 0.6f;
    else if (t>=0.015 && t<.02){
	x_val = 0.8f;

	if (play1.pos[0] + (1.7*play1.width) >= play2.pos[0] && t > .019)
	{
	    hit = true;
	    punch1 = false;
	    clk = true;
	}

    } else {
	punch1 = false;
	clk = true;


    }

    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, play1Texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.1f);
    glColor4ub(255,255,255,255);

    glBegin(GL_QUADS);
    glTexCoord2f(x_val, 0.5f); glVertex2i(-w, -h);
    glTexCoord2f(x_val, 0.0f); glVertex2i(-w, h);
    glTexCoord2f(x_val + 0.2, 0.0f); glVertex2i(w, h);
    glTexCoord2f(x_val + 0.2, 0.5f); glVertex2i(w, -h); 
    glEnd();
    glPopMatrix();



}

