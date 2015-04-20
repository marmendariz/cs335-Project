
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
void init_players(void);
void init_healthBars(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

int xres=1280, yres=680;
int leftButtonDown=0;
Vec leftButtonPos;

bool punch = false;

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
	init_opengl();
	init_players();
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

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}

void init_players(void)
{
	/*Player ONE*/
	play1.pos[0] = 350;
	play1.pos[1] = 200;
	play1.vel[0] = 0.0;
	play1.vel[1] = 0.0;
	play1.width = 70.0;
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
    play2.width = 70.0;
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

	play1.atk.posPunch[0] = play1.pos[0] + play1.width;	
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
	while(play1.pos[0]+2*play1.width>=play2.pos[0])
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

	
    while(play1.pos[0] - play1.width  < 0){
	play1.pos[0] += 2;
    }

    while(play2.pos[0] + play2.width > xres) {
	play2.pos[0] -= 2;
    }
    
}

void drawBox(Flt width, Flt height)
{
	int w = width;
	int h = height;
	glBegin(GL_QUADS);
	glVertex2i(-w,-h);
    glVertex2i(-w, h);
    glVertex2i( w, h);
    glVertex2i( w,-h);
    glEnd();

} 
void render(void)
{
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);

	/*Draw Boxes*/
	glColor3ub(30,60,90);
	glPushMatrix();
	glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);
	drawBox(play1.width,play1.height);
	glPopMatrix();

	glColor3ub(30,60,90);
	glPushMatrix();
	glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);
	drawBox(play2.width,play2.height);
	glPopMatrix();

	/*Draw Healthbars*/

	glColor3ub(0,0,0);
	glPushMatrix();
	glTranslatef(play1.hbar.posOut[0], play1.hbar.posOut[1], play1.hbar.posOut[2]);
	drawBox(play1.hbar.wOut,play1.hbar.hOut);
	glPopMatrix();


	glColor3ub(0,0,0);
	glPushMatrix();
	glTranslatef(play2.hbar.posOut[0], play2.hbar.posOut[1], play2.hbar.posOut[2]);
	drawBox(play2.hbar.wOut,play2.hbar.hOut);
	glPopMatrix();

	glColor3ub(200,0,0);
	glPushMatrix();
	glTranslatef(play1.hbar.pos[0], play1.hbar.pos[1], play1.hbar.pos[2]);
	drawBox(play1.hbar.width,play1.hbar.height);
	glPopMatrix();

	glColor3ub(200,0,0);
	glPushMatrix();
	glTranslatef(play2.hbar.pos[0], play2.hbar.pos[1], play2.hbar.pos[2]);
	drawBox(play2.hbar.width,play2.hbar.height);
	glPopMatrix();

	if(keys[XK_f]){
	    glColor3ub(200,0,0);
	    glPushMatrix();
	    glTranslatef(play1.atk.posPunch[0], play1.atk.posPunch[1], play1.atk.posPunch[2]);
	    drawBox(play1.atk.wPunch,play1.atk.hPunch);
	    glPopMatrix();

	    if(play1.atk.posPunch[0] + play1.atk.wPunch*1.5 >= play2.pos[0])
	    {
		play2.pos[0] += 100;
	    }

	}

	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	//ggprint8b(&r, 16, 0x0000000, "Ultimate Punch Frenzy: Immortal Peacetime: Fighters in the Street");
	//ggprint8b(&r, 16, 0x0000000, "Arrows/WASD to move");
	//ggprint8b(&r, 16, 0x0000000, "S - Slow down movement");
	//
	r.center = 1;
	r.left = play1.pos[0];
	r.bot  = play1.pos[1]-4;
	ggprint8b(&r, 11, 0x00ffff00, "Player One");
	r.left = play2.pos[0];
	r.bot  = play2.pos[1]-4;
	ggprint8b(&r, 11, 0x00ffff00, "Player Two");
}



