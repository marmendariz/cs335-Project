
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
void init_balls(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

int xres=1280, yres=680;
int leftButtonDown=0;
Vec leftButtonPos;
typedef struct t_Ball {
	Vec pos;
	Vec vel;
        Vec center;
	float radius;
	float mass;
        float width,height;
} Ball;
Ball ball1;
Ball ball2;

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
	init_balls();
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
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
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
	//else {
	//	// %p creates hexadecimal output like in glxinfo
	//	printf("\n\tvisual %p selected\n", (void *)vi->visualid);
	//}
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
	//
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

void init_balls(void)
{
	/*Player ONE*/
	ball1.pos[0] = 350;
	ball1.pos[1] = 200;
	ball1.vel[0] = 0.0;
	ball1.vel[1] = 0.0;
	ball1.width = 70.0;
    ball1.height= 120.0;
    ball1.radius= 30.0;
	ball1.mass = 1.0;

	/*Player TWO*/
	ball2.pos[0] = xres - 350;
	ball2.pos[1] = 200;
	ball2.vel[0] = 0.0;
	ball2.vel[1] = 0.0;
	ball2.radius = 30.0;
    ball2.width = 70.0;
    ball2.height= 120.0;
	ball2.mass = 1.0;
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
				ball1.vel[0] -= 1.0;
				break;
			case XK_a:
				ball2.vel[0] -= 1.0;
				break;
		    case XK_s:
				ball2.vel[1] -= 1.0;
				break;
                        case XK_d:
				ball2.vel[0] += 1.0;
				break;
                       case XK_w:
				ball2.vel[1] += 1.0;
				break;
			case XK_Right:
				ball1.vel[0] += 1.0;
				break;
			case XK_Up:
				ball1.vel[1] += 1.0;
				break;
			case XK_Down:
				ball1.vel[1] -= 1.0;
				break;
			case XK_x:
				//press s to slow the balls
				ball1.vel[0] *= 0;
				ball1.vel[1] *= 0;
				ball2.vel[0] *= 0;
				ball2.vel[1] *= 0;
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
	Flt tx, ty;
	Flt movi[2], movj[2];
	Flt massFactor, massi, massj;
	Vec vcontact[2];
	Vec vmove[2];
	Flt dot0,dot1;
	tx = ball1.pos[0] - ball2.pos[0];
	distance = sqrt(tx * tx);
	double stepVel = 4.0;

	/*CHECK KEYS*/
	/*Player ONE*/
	if(keys[XK_a])
		ball1.pos[0] -= stepVel;	
	if(keys[XK_d])
		ball1.pos[0] += stepVel;

	/*Player TWO*/
	if(keys[XK_Left])
		ball2.pos[0] -= stepVel;
	if(keys[XK_Right])
		ball2.pos[0] += stepVel;
	/*END CHECK KEYS*/

	if (distance < ball1.width+(ball2.width)) {
		//We have a collision!
		//vector from center to center.
		vcontact[0][0] = tx;
		//vcontact[0][1] = ty;
		vcontact[0][2] = 0.0;
		VecNormalize(vcontact[0]);
		VecCopy(vcontact[0],vcontact[1]);
		VecNegate(vcontact[1]);
		movi[0] = ball1.vel[0];
		movi[1] = ball1.vel[1];
		movj[0] = ball2.vel[0];
		movj[1] = ball2.vel[1];
		vmove[0][0] = movi[0];
		//vmove[0][1] = movi[1];
		vmove[0][2] = 0.0;
		vmove[1][0] = movj[0];
		//vmove[1][1] = movj[1];
		vmove[1][2] = 0.0;
		VecNormalize(vmove[0]);
		VecNormalize(vmove[1]);
		//Determine how direct the hit was...
		dot0 = VecDot(vcontact[0], vmove[0]);  //dot product
		dot1 = VecDot(vcontact[1], vmove[1]);  //dot product
		//Find the closing (relative) speed of the objects...
		float speed0 = 0.5;
                //sqrtf( movi[0]*movi[0] + movi[1]*movi[1]) * dot0;
		float speed1 = 0.5;
                //sqrtf( movj[0]*movj[0] + movj[1]*movj[1]) * dot1;
		speed = speed0 + speed1;
		if (speed < 0.0) {
			//Normalize the mass of each object...
			massFactor = 2.0 / (ball1.mass + ball2.mass);
			massi = ball1.mass * massFactor;
			massj = ball2.mass * massFactor;
			ball2.vel[0] += vcontact[0][0] * speed * massi;
			ball2.vel[1] += vcontact[0][1] * speed * massi;
			ball1.vel[0] += vcontact[1][0] * speed * massj;
			ball1.vel[1] += vcontact[1][1] * speed * massj;
		}
	}

	//Update position
	while(ball1.pos[0]+2*ball1.width>=ball2.pos[0])
	{
		ball1.pos[0]-= 2;
        ball2.pos[0]+= 2;
    }
    if(ball1.pos[0]<(30+ball2.pos[0]))
    {
    	ball1.pos[0] += ball1.vel[0];
    	ball2.pos[0] += ball2.vel[0];
    }

    //steps
    ball1.vel[0] = 0;
    ball2.vel[0] = 0;

	//Check for collision with window edges
	if ((ball1.pos[0] < ball1.width && ball1.vel[0] < 0.0) ||
		(ball1.pos[0] >= (Flt)xres-ball1.width && ball1.vel[0] > 0.0)) {
		ball1.vel[0] = -ball1.vel[0];
	}
	if ((ball1.pos[1] < ball1.width && ball1.vel[1] < 0.0) ||
		(ball1.pos[1] >= (Flt)yres-ball1.width && ball1.vel[1] > 0.0)) {
		ball1.vel[1] = -ball1.vel[1];
	}
	if ((ball2.pos[0] < ball2.radius && ball2.vel[0] < 0.0) ||
		(ball2.pos[0] >= (Flt)xres-ball2.radius && ball2.vel[0] > 0.0)) {
		ball2.vel[0] = -ball2.vel[0];
	}
	if ((ball2.pos[1] < ball2.radius && ball2.vel[1] < 0.0) ||
		(ball2.pos[1] >= (Flt)yres-ball2.radius && ball2.vel[1] > 0.0)) {
		ball2.vel[1] = -ball2.vel[1];
	}
}

void drawBall(Flt rad)
{
	int i;
	static int firsttime=1;
	static Flt verts[32][2];
	static int n=32;
	if (firsttime) {
		Flt ang=0.0;
		Flt inc = 3.14159 * 2.0 / (Flt)n;
		for (i=0; i<n; i++) {
			verts[i][0] = sin(ang);
			verts[i][1] = cos(ang);
			ang += inc;
		}
		firsttime=0;
	}
	glBegin(GL_TRIANGLE_FAN);
		for (i=0; i<n; i++) {
			glVertex2f(verts[i][0]*rad, verts[i][1]*rad);
		}
	glEnd();
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
	//
	//draw balls
	glColor3ub(30,60,90);
	glPushMatrix();
	glTranslatef(ball1.pos[0], ball1.pos[1], ball1.pos[2]);
	drawBox(ball1.width,ball1.height);
	glPopMatrix();

	glColor3ub(30,60,90);
	glPushMatrix();
	glTranslatef(ball2.pos[0], ball2.pos[1], ball2.pos[2]);
	drawBox(ball2.width,ball2.height);
	glPopMatrix();
	//
	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x0000000, "Ultimate Punch Frenzy: Immortal Peacetime: Fighters in the Street");
	ggprint8b(&r, 16, 0x0000000, "Arrows/WASD to move");
	//ggprint8b(&r, 16, 0x0000000, "S - Slow down movement");
	//
	r.center = 1;
	r.left = ball1.pos[0];
	r.bot  = ball1.pos[1]-4;
	ggprint8b(&r, 11, 0x00ffff00, "Player One");
	r.left = ball2.pos[0];
	r.bot  = ball2.pos[1]-4;
	ggprint8b(&r, 11, 0x00ffff00, "Player Two");
}



