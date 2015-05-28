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
#include "audio.h"
#include "ppm.h"
#include "log.h"
#include "defs.h"
#include "KevinO.h"
#include "MarkA.h"
#include "SarahbethR.h"
#include "ZenaidaG.h"
extern "C" {
#include "fonts.h"
}

#define MakeVector(x,y,z,v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a) (a)[0]=(-(a)[0]); (a)[1]=(-(a)[1]); (a)[2]=(-(a)[2]);
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];

/*X Windows global variables*/
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
void init_character_select(void);
void render(void);
void animatePlayerOne(Flt, Flt);
void drawmenu_button(Flt, Flt);
void init_menu();
void menu_render();

void restart_game(void);


int xres=1280, yres=680;
//int leftButtonDown=0;
int punchDamage = 10;
//Vec leftButtonPos;

bool two_players = false;
bool play_game = false;
bool go_selchar=false;
bool sound_p = false;

bool visted =false;
bool selectedBack=false;
bool streetback=false;
bool forestback=false;
#ifdef USE_SOUND
int play_sounds = 0;
#endif

Player play1;
Player play2;

/*TEXTURE STUFF*/

Ppmimage *play2guile=NULL;
GLuint play2guiletext;
Ppmimage *play1guile=NULL;
GLuint play1guiletext;
Ppmimage *play1bguile=NULL;
GLuint play1bguiletext;
Ppmimage *play2bguile=NULL;
GLuint play2bguiletext;
Ppmimage *play1red=NULL;
GLuint play1redtext;
Ppmimage *play2red=NULL;
GLuint play2redtext;
Ppmimage *play1joker=NULL;
GLuint play1jokertext;
Ppmimage *play2joker=NULL;
GLuint play2jokertext;


Ppmimage *metalImage=NULL;
GLuint metalTexture;

Ppmimage *metal2Image=NULL;
GLuint metal2Texture;
Flt metal2Pos[4];
Flt metal2Dim[4];  

Ppmimage *forestImage=NULL;
GLuint forestTexture;
int forest =1;
int street=1;

Ppmimage *streetImage=NULL;
GLuint streetTexture;

Ppmimage *titleImage=NULL;
int title =1;
GLuint titleTexture;


Ppmimage *selectcharacter_Image=NULL;
int selchar=1;
GLuint selectTexture;

/*Holds character names*/
char names[4][30];

//character select
bool player1choose=false;
bool player2choose=false;
bool player1guile=false;
bool player1bguile=false;
bool player2guile=false;
bool player2bguile=false;
bool player1red=false;
bool player2red=false;
bool player1joker=false;
bool player2joker=false;
int player1=0;
int player2=0;

int please=0;
bool restart=false;
bool playone=true;
extern int q;

/***********Setup timers **********/
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
/**************************************/


int i=0;

//VARIABLES FOR SARAHBETH... NO TOUCHY!!!!!
clock_t startTime;
bool readyPrompt = false;
bool readyPromptClk = true;
//---------------------------//

/***************MAIN*******************/
int main(void)
{
    initXWindows();

    init_opengl();

    init_character_boxes();
    init_menu();
    init_character_select();
    init_players();
    init_music();
    init_sound();

    //init_opengl();
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

        if(play_game == false && two_players==false && selectedBack ==false){
            if(i==0){

                goodbye();

                sound_p=true;
                let_the_music_play(1);
                i++;
            }
            menu_render();


            if(go_selchar == true){
                if(i==1)
                { 
                    goodbye();
                    i++;
                    /*if (visted==false){
                      sound_p=false;
                      let_the_music_play(1);
                      }
                     */
                    sound_p= true;
                    let_the_music_play(15);


                    play_sounds=0;
                    soundeffects(9);
                    //play_sounds=1;

                }

                character_select_render();

                if (player1choose==true && player2choose==true && selectedBack==false){
                    //please++;
                    printf("two players\n");
                    two_players=true;
                    printf("two players\n");
                    if(selectedBack==true){
                        printf("background selected");
                        init_opengl();
                    }

                }
            }
        }
        else if(play_game==false && two_players == true && go_selchar==false && selectedBack==true){

            //goodbye();
            //if(sound_p==true){
            //sound_p=false;
            //let_the_music_play(14);

            //}
            menu_render();
            //if(restart==true){
            //	character_select_render();
            //}

        }
        else if(play_game ==true && two_players==true && go_selchar==false && selectedBack==true){
            if(i==2)
            {

                goodbye();
                //sound_p=false;
                //let_the_music_play(0);
                i++;
                sound_p=true;
                int rd = (rand()%3)+12;
                let_the_music_play(rd);

            }
            play_sounds=0;
            render();
        }
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
    /*Set the window title bar*/
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
    /*window has been resized*/
    setup_screen_res(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, xres, 0, yres, -1, 1);
    set_title();
}

void init_menu(void)
{

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

    /*Initialize title screen image*/
    char x[] = "./images/title.ppm";
    titleImage = ppm6GetImage(titleImage,x);
    glGenTextures(1, &titleTexture);
    glBindTexture(GL_TEXTURE_2D, titleTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            titleImage->width, titleImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, titleImage->data);

    strcpy(names[0],"Guile");
    strcpy(names[1],"Bizarro Guile");
    strcpy(names[2],"Eliug");
    strcpy(names[3],"Joker");
}  

void init_character_select(void)
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

    char a[] = "./images/characterselect.ppm";
    selectcharacter_Image = ppm6GetImage(selectcharacter_Image,a);
    glGenTextures(1, &selectTexture);
    glBindTexture(GL_TEXTURE_2D, selectTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
            selectcharacter_Image->width, selectcharacter_Image->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, selectcharacter_Image->data);
}

void init_players(void)
{
    /*Initialize player position & size*/
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

    play1.punch = false;
    play1.finPunch = false;
    play1.control = false;
    play1.punchClk = true;
    play1.punchHit = false;
    play1.draw = true;
    play1.walkClk = true;
    play1.walk = false;
    play1.block = false;
    play1.blockClk = true;

    /*Player TWO*/
    play2.pos[0] = xres - 350;
    play2.pos[1] = 200;
    play2.vel[0] = 0.0;
    play2.vel[1] = 0.0;
    play2.radius = 30.0;
    play2.width = 120.0;
    play2.height= 120.0;
    play2.mass = 1.0;

    play2.punch = false;
    play2.finPunch = false;
    play2.control = false;
    play2.punchClk = true;
    play2.punchHit = false;
    play2.draw = true;
    play2.walkClk = true;
    play2.walk = false;
    play2.block = false;
    play2.blockClk = true;


    init_healthBars();
}

void init_healthBars()
{
    /*Initialize position and size of 
     * healthbars*/
    metal2Pos[0] = play1.hbar.pos[0] = 250;
    metal2Pos[1] = play1.hbar.pos[1] = 620;
    metal2Dim[0] = play1.hbar.width = 235;
    metal2Dim[1] = play1.hbar.height = 20;

    play1.hbar.posOut[0] = 250;
    play1.hbar.posOut[1] = 620;
    play1.hbar.wOut = 240;
    play1.hbar.hOut =25;

    metal2Pos[2] = play2.hbar.pos[0] = xres - 250;
    metal2Pos[3] = play2.hbar.pos[1] = 620;
    metal2Dim[2] = play2.hbar.width = 235;
    metal2Dim[3] = play2.hbar.height = 20;

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
    /*Check for collision here*/
    Flt distance, speed;
    Flt tx;
    Flt movi[2], movj[2];
    Flt massFactor, massi, massj;
    Vec vcontact[2];
    Vec vmove[2];
    tx = play1.pos[0] - play2.pos[0];
    distance = sqrt(tx * tx);
    double stepVel = 4.0;

    /*CHECK KEYS*/
    /*Player ONE*/

    if(play1.control && !(play1.hbar.width <=0)){

        if(!play1.block){
            if(!(keys[XK_a] && keys[XK_d])){
		if(!keys[XK_f]){
                if(keys[XK_a] == 1){
                    play1.pos[0] -= stepVel;
                    if(play1.walkClk)
                        play1.beginWalk = clock();
                    play1.walk = true;
                }
                if(keys[XK_d] == 1){
                    play1.pos[0] += stepVel;
                    if(play1.walkClk)
                        play1.beginWalk = clock();
                    play1.walk = true;
                }
		}
            }
            if(keys[XK_d] == 0 && keys[XK_a] == 0){
                play1.walkClk = true;
                play1.draw = true;
                play1.walk = false;
            }       

            if(keys[XK_f] == 1){
                play1.punch = true;
	    }
	    if(!keys[XK_f])
		play1.punchClk = true;
        }

        if(!play1.punch && !play1.walk){
            if(keys[XK_g] == 1){

                if(play1.blockClk)
                    play1.beginBlock = clock();
                play1.block = true;
            }

            if(keys[XK_g] == 0){

                play1.blockClk = true;
                play1.block = false;
            }
        }
    }
    /*Player TWO*/

    if(play2.control && !(play2.hbar.width <= 0)){

        if(!play2.block){
            if(!(keys[XK_Left] && keys[XK_Right])){
		if(!keys[XK_l]){
                if(keys[XK_Left]){
                    play2.pos[0] -= stepVel;
                    if(play2.walkClk)
                        play2.beginWalk = clock();
                    play2.walk = true;
                }

                if(keys[XK_Right]){
                    play2.pos[0] += stepVel;
                    if(play2.walkClk)
                        play2.beginWalk = clock();
                    play2.walk = true;
                }
		}
            }

            if(keys[XK_Left] == 0 && keys[XK_Right] == 0){
                play2.walkClk = true;
                play2.draw = true;
                play2.walk = false;
            }       

            if(keys[XK_l])
                play2.punch = true;
	    if(!keys[XK_l])
		play2.punchClk = true;
        }

        if(!play2.punch && !play2.walk){

            if(keys[XK_k] == 1){

                if(play2.blockClk)
                    play2.beginBlock = clock();
                play2.block = true;
            }

            if(keys[XK_k] == 0){

                play2.blockClk = true;
                play2.block = false;
            }
        }

    }

    /*END CHECK KEYS*/

    play1.atk.posPunch[0] = play1.pos[0]; 
    play1.atk.posPunch[1] = play1.pos[1] + play1.height - 100;

    if (distance < play1.width+(play2.width)) {
        //We have a collision!
        //vector from center to center.
        vcontact[0][0] = tx;
        vcontact[0][2] = 0.0;
        VecNormalize(vcontact[0]);
        VecCopy(vcontact[0],vcontact[1]);
        VecNegate(vcontact[1]);
        movi[0] = play1.vel[0];
        movi[1] = play1.vel[1];
        movj[0] = play2.vel[0];
        movj[1] = play2.vel[1];
        vmove[0][0] = movi[0];
        vmove[0][2] = 0.0;
        vmove[1][0] = movj[0];
        vmove[1][2] = 0.0;
        VecNormalize(vmove[0]);
        VecNormalize(vmove[1]);
        //Determine how direct the hit was...
        //Find the closing (relative) speed of the objects...
        float speed0 = 0.5;
        float speed1 = 0.5;
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

    //Update position
    while(play1.pos[0]+play1.width>=play2.pos[0]+play2.width)
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
    int w = width, h = height;

    /*x= 1 or 3 - Bind player1 and player2 textures*/
    if(x==1)
    {
        glEnable(GL_TEXTURE_2D);

        if(player1guile==true && player1bguile==false && player1red==false && player1joker==false){
            glBindTexture(GL_TEXTURE_2D, play1guiletext);
        }
        else if(player1guile==false && player1bguile==true && player1red==false && player1joker==false){
            glBindTexture(GL_TEXTURE_2D, play1bguiletext);
        }
       else if(player1guile==false && player1bguile==false && player1red==true && player1joker==false){
            glBindTexture(GL_TEXTURE_2D, play1redtext);
       }
       else if(player1guile==false && player1bguile==false && player1red==false && player1joker==true){
            glBindTexture(GL_TEXTURE_2D, play1jokertext);
        }
        
	glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0.1f);
        glColor4ub(255,255,255,255);
    }
    if(x==3)
    {
        glEnable(GL_TEXTURE_2D);

      //  if(player2guile==true && player2bguile==false){
      //      glBindTexture(GL_TEXTURE_2D, play2guiletext);
       // }
       // else if(player2guile==false && player2bguile==true){
       //     glBindTexture(GL_TEXTURE_2D, play2bguiletext);
       // }
        //glBindTexture(GL_TEXTURE_2D, play2Texture);

      if(player2guile==true && player2bguile==false && player2red==false && player2joker==false){
            glBindTexture(GL_TEXTURE_2D, play2guiletext);
        }
        else if(player2guile==false && player2bguile==true && player2red==false && player2joker==false){
            glBindTexture(GL_TEXTURE_2D, play2bguiletext);
        }
       else if(player2guile==false && player2bguile==false && player2red==true && player2joker==false){
            glBindTexture(GL_TEXTURE_2D, play2redtext);
       }
       else if(player2guile==false && player2bguile==false && player2red==false && player2joker==true){
            glBindTexture(GL_TEXTURE_2D, play2jokertext);
        }
   
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER,0.1f);
        glColor4ub(255,255,255,255);
    }

    /*x=0, bind metal texture for healthbar*/
    if(x==0)
        glBindTexture(GL_TEXTURE_2D, metalTexture);
    if(x==4)
        glBindTexture(GL_TEXTURE_2D, metal2Texture);

    /*If x=2, Draw red bars - No texture*/
    if(x==2)
        glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    /*Draw player1/player2 with texture*/
    if(x==1 || x==3)
    {
        glTexCoord2f(0.0f, 1.0f/3.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(0.1f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(0.1f, 1.0f/3.0f); glVertex2i(w, -h);    
    }
    /*Draw regular full size of texture*/
    if(x==0 || x==4)
    {
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-w, -h);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, h);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(w, h);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(w, -h);	
    }
    /*Draw non-textured rectangle*/
    else
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
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_TEXTURE_2D);
    if(forestback==true && streetback==false){
        if (forest) {
            glBindTexture(GL_TEXTURE_2D, forestTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
            glEnd();
        }
    }
    else if(forestback==false && streetback==true){
        if (street) {
            glBindTexture(GL_TEXTURE_2D, streetTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
            glEnd();
        }
    }

    /*************************************/
    /*Draw Player One*/
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(play1.pos[0], play1.pos[1], play1.pos[2]);
    if(play1.draw)
        drawBox(play1.width,play1.height,1);
    glPopMatrix();
    /*Draw Player Two*/
    glPushMatrix();
    glTranslatef(play2.pos[0], play2.pos[1], play2.pos[2]);
    if(play2.draw)
        drawBox(play2.width,play2.height,3);
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

    /*Draw inner metal frames second*/
    glDisable(GL_ALPHA_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(metal2Pos[0], metal2Pos[1], 0.0);
    drawBox(metal2Dim[0],metal2Dim[1],4);
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(metal2Pos[2], metal2Pos[3], 0.0);
    drawBox(metal2Dim[2],metal2Dim[3],4);
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
    if(readyPrompt == true){
        if(readyPromptClk == true){
            startTime = clock();
            readyPromptClk = false;
        }
        animate_ReadySetGo();
    }

    /*****************Player 1 punch and hit******************/

    if((play1.punch && !play1.punchHit) || play1.finPunch)
    {
        if(play1.punchClk)
        {
            play1.beginPunch = clock();

        }
        play1.control = false;
        play1.draw = false;
        animatePlayerOne(play1.width,play1.height);
    }

    if(!play1.punch && play1.punchHit){
        if(!play2.block){
	play2.pos[0] += 100;
        play2.hbar.pos[0] += punchDamage;
        play2.hbar.width -= punchDamage;
	}
	else{
	    play2.pos[0]+=10;
	    play2.hbar.pos[0]+=punchDamage*0.2;
	    play2.hbar.width-=punchDamage*0.2;
	}
        int punchn=(rand()%3)+2;

        //play_sounds=0;
        soundeffects(punchn);
        //play_sounds=1;
        //soundeffects(punchn);

        play1.punchHit = false;
    }

    if(play1.walk && play1.control && !play1.block){
        animatePlayerOneWalk(play1.width, play1.height);
    }

    if(play1.block){
        animatePlayerOneBlock(play1.width, play1.height);
    }

    if(play2.block){
        animatePlayerTwoBlock(play2.width, play2.height);
    }

    if(play2.walk && play2.control){
        animatePlayerTwoWalk(play2.width, play2.height);
    }

    /*****************Player 2 punch and hit******************/
    if((play2.punch && !play2.punchHit) || play2.finPunch)
    {
        if(play2.punchClk)
        {
            play2.beginPunch = clock();

        }
        play2.control = false;
        play2.draw = false;
        animatePlayerTwo(play2.width,play2.height);
    }

    if(!play2.punch && play2.punchHit){
	if(!play1.block){
        play1.pos[0] -= 100;
        play1.hbar.pos[0] -= punchDamage;
        play1.hbar.width -= punchDamage;
	}
	else{
	    play1.pos[0]-=10;
	    play1.hbar.pos[0]-=punchDamage*0.2;
	    play1.hbar.width-=punchDamage*0.2;
	}
        int punchn=(rand()%3)+2;

        play_sounds=0;
        soundeffects(punchn);
        //play_sounds=1;
        //soundeffects(punchn);

        play2.punchHit = false;
    }

    /*healthbar check****************************/

    if(play2.hbar.width <= 0){
        play2.hbar.width = 0;

        //punchDamage = 0;
        play1.control=false;
        play2.control=false;
	play2.block = false;
	play2.punch = false;
	play2.walk = false;	
	play1.block = false;
	play1.punch = false;
	play1.walk = false;
        play1.draw=true;
        play2.draw=true;

        glColor3f(0.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(640, 400, 0.0);
        drawBox(230,100,5);
        glPopMatrix();

        if(playone==true){
            play_sounds=0;
            soundeffects(10);
            playone=false;
        }
        //play_sounds=1;
        //soundeffects(11);
    }
    if(play1.hbar.width <= 0){
        play1.hbar.width = 0;
        //punchDamage = 0;
        play1.control=false;
        play2.control=false;
        play1.draw=true;
        play2.draw=true;
	play1.block = false;
	play1.punch = false;
	play1.walk = false;
	play2.block = false;
	play2.punch = false;
	play2.walk = false;

        glColor3f(0.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(640, 400, 0.0);
        drawBox(230,100,5);
        glPopMatrix();
        if(playone==true){
            play_sounds=0;
            soundeffects(11);
            playone=false;
        }
        //play_sounds=1;
        //soundeffects(11);


    }

    glEnable(GL_TEXTURE_2D);
    r.bot = yres - 20;
    r.center = 5;
    r.left = play1.hbar.posOut[0]-180;
    r.bot  = play1.hbar.posOut[1]-10;
    ggprint16(&r, 20, 0x00ffff00, play1.name);

    r.left = play2.hbar.posOut[0]+180;
    r.bot  = play2.hbar.posOut[1]-10;
    ggprint16(&r, 20, 0x00ffff00, play2.name);
}



