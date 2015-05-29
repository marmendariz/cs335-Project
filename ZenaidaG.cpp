#include "ZenaidaG.h"
#include "audio.h"

extern void init_healthBars();
extern void init_players();
extern bool readyPrompt;
extern bool two_players;// = false;
extern bool play_game; //= false;
extern bool go_selchar;//r=false;
extern bool sound_p;// = false;

extern bool visted;// =false;
extern bool selectedBack;//=false;
extern bool streetback;//=false;
extern bool forestback;//=false;

extern bool player1choose;//=false;
extern bool player2choose;//=false;
extern bool player1guile;//=false;
extern bool player1bguile;//=false;
extern bool player2guile;//=false;
extern bool player2bguile;//=false;
extern bool player1red;//=false;
extern bool player2red;//=false;
extern bool player1joker;//=false;
extern bool player2joker;//=false;
extern int player1;//=0;
extern int player2;//=0;
extern int forest;
extern int street;
extern int i;
extern int please;//=0;
extern bool restart;//=false;
extern bool playone;//=true;
extern int q;
extern char names[4][30];
extern int keys[65536];
bool help_screen=false;
extern Player play1;
extern Player play2;

void restart_game(void){
    init_healthBars();
    init_players();

    help_screen=false;
    go_selchar=true;
    play_game=false;
    two_players=false;
    player1=0;
    player2=0;
    player1guile=false;
    player1bguile=false;
    player2guile=false;
    player2bguile=false;
    player1red=false;
    player2red=false;
    player2joker=false;
    player1joker=false;
    player1choose=false;
    player2choose=false;
    i=1;
    q=0;
    visted=true;
    selectedBack=false;
    forestback=false;
    streetback=false;
    restart=true;
    goodbye();
    playone=true;
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
		help_screen=false;

            //goodbye();

            if (play_game == true) { // they are playing 
                play_game = false;
                play1.control = false;
                play2.control = false;
            }
            else if (play_game == false) {
                //at menu screen need to select characters
                if(go_selchar==false && two_players==false && selectedBack==false)
                    go_selchar=true;

                //selected characters go to game
                else if(go_selchar==true && two_players==true && selectedBack==true){
                    go_selchar=false;
                    play_game =true;
                    readyPrompt = true;
                }

                //in game 
                else if(go_selchar==false && two_players==true && selectedBack==true){
                    play_game = true;
                    play1.control = true;
                    play2.control = true;
                }
            }
            break;

        case XK_r:
            if(play_game==false && two_players == true && go_selchar==false){
                restart_game();
            }

            break;

	case XK_h:
		
            if((play_game==false && two_players == true && go_selchar==false && selectedBack==true) ||
	    (play_game==false && two_players == false && go_selchar==false && selectedBack==false)){
	         if(help_screen==true)
		 	help_screen=false;
		else if(help_screen==false)
			help_screen=true;
            }
		break;
            /*Use numbers to select players*/
        case XK_1:
            if(go_selchar==true){
                if(player1choose==false && player2choose==false){
                    player1=1;
                    player1guile=true;
                    player1bguile=false;
		    player1red=false;
		    player1joker=false;
                    strcpy(play1.name,names[0]);

                    player1choose=true;
                }
                else if(player1choose==true && player2choose==false){

                    player2=1;
                    player2guile=true;
                    player2bguile=false;
		    player2red=false;
		    player2joker=false;
                    strcpy(play2.name,names[0]);
                    player2choose=true;
                }
            }
            break;
        case XK_2:
            if(go_selchar==true){
                if(player1choose==false && player2choose==false){

                    player1bguile=true;
                    player1guile=false;
		    player1red=false;
		    player1joker=false;
                    player1=2;
                    strcpy(play1.name,names[1]);
                    player1choose=true;
                }
                else if(player1choose==true && player2choose==false){
                    player2guile=false;
                    player2bguile=true;
		    player2red=false;
		    player2joker=false;
                    player2=2;
                    strcpy(play2.name,names[1]);
                    player2choose=true;

                }
            }
        break;

	case XK_3:
            if(go_selchar==true){
                if(player1choose==false && player2choose==false){

                    player1red=true;
		    player1bguile=false;
                    player1joker=false;
                    player1guile=false;
                   
		    player1=3;
                    strcpy(play1.name,names[2]);
                    player1choose=true;
                }
                else if(player1choose==true && player2choose==false){
                    player2guile=false;
                    player2bguile=false;
		    player2red=true;
		    player2joker=false;
                   
		    player2=3;
                    strcpy(play2.name,names[2]);
                    player2choose=true;

                }
            }
            break;

	case XK_4:
            if(go_selchar==true){
                if(player1choose==false && player2choose==false){

                    player1red=false;
		    player1bguile=false;
                    player1joker=true;
                    player1guile=false;
                   
		    player1=4;
                    strcpy(play1.name,names[3]);
                    player1choose=true;
                }
                else if(player1choose==true && player2choose==false){
                    player2guile=false;
                    player2bguile=false;
		    player2red=false;
		    player2joker=true;
                   
		    player2=4;
                    strcpy(play2.name,names[3]);
                    player2choose=true;

                }
            }
            break;

        case XK_0:
		  if(go_selchar==true && selectedBack==false && two_players==true){

                streetback=true;
                forestback=false;
                selectedBack=true;

            }
            break;
        
		case XK_9:
			if(go_selchar==true && selectedBack==false && two_players==true ){
                streetback=false;
                forestback=true;
                selectedBack=true;
       		}
		     break;
}
    return 0;
}



