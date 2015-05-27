#include "ZenaidaG.h"
extern bool sound_p;
#ifdef USE_SOUND
extern int play_sounds;
#endif

void init_music(void)
{
    char select[]= "./sounds/zfc.mp3";
    char start[] ="./sounds/blue.mp3";

    char fight1[]="./sounds/doubt.mp3";
    char fight2[]="./sounds/thefall.mp3";
    char fight3[]="./sounds/Takedown.mp3";
 
#ifdef USE_SOUND
        if(fmod_init()){
                printf("ERROR-fmod_init()\n\n");
                return;
        }

        if(fmod_createsound(select,15)){

                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
        if(fmod_createsound(start,1)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }

	if(fmod_createsound(fight1,12)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
       if(fmod_createsound(fight2,13)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
	if(fmod_createsound(fight3,14)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }



        fmod_setmode(15,FMOD_LOOP_NORMAL);
        fmod_setmode(1,FMOD_LOOP_NORMAL);
        fmod_setmode(12,FMOD_LOOP_NORMAL);
        fmod_setmode(13,FMOD_LOOP_NORMAL);
	fmod_setmode(14,FMOD_LOOP_NORMAL);
#endif
}

void init_sound()
{
	char punch1[]="./sounds/punch1.mp3";
	char punch2[]="./sounds/punch2.mp3";
	char punch3[]="./sounds/punch3.mp3";

	char fistswing1[]="./sounds/fistswing1.mp3";
	char fistswing2[]="./sounds/fistswing2.mp3";

	char block1[]="./sounds/block1.mp3";
	char block2[]="./sounds/block2.mp3";


        char player1wins[]="./sounds/play1wins.mp3";
	char player2wins[]="./sounds/play2wins.mp3";
	char selectchar[]="./sounds/selectcharacter.mp3";
        char ready[]="./sounds/ready.mp3";
#ifdef USE_SOUND
        if(fmod_init()){
                printf("ERROR-fmod_init()\n\n");
                return;
        }

	//punch sounds
        if(fmod_createsound(punch1,2)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
        if(fmod_createsound(punch2,3)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}
	if(fmod_createsound(punch3,4)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }

	//fist swing sounds
	if(fmod_createsound(fistswing1,5)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
        if(fmod_createsound(fistswing2,6)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}
	
	//block sounds
	if(fmod_createsound(block1,7)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }
        if(fmod_createsound(block2,8)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}

	//kevins voice
	if(fmod_createsound(selectchar,9)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }

        if(fmod_createsound(player1wins,10)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}
	if(fmod_createsound(player2wins,11)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}
	if(fmod_createsound(ready,16)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
	}




        fmod_setmode(2,FMOD_LOOP_OFF);
        fmod_setmode(3,FMOD_LOOP_OFF);
        fmod_setmode(4,FMOD_LOOP_OFF);
        fmod_setmode(5,FMOD_LOOP_OFF);
        fmod_setmode(6,FMOD_LOOP_OFF);
        fmod_setmode(7,FMOD_LOOP_OFF);
        fmod_setmode(8,FMOD_LOOP_OFF);
        fmod_setmode(9,FMOD_LOOP_OFF);

        fmod_setmode(10,FMOD_LOOP_OFF);
        fmod_setmode(11,FMOD_LOOP_OFF);
        fmod_setmode(16,FMOD_LOOP_OFF);

#endif

}

void soundeffects(int num)
{    
     if(play_sounds==0)
     	fmod_playeffect(num);
     
     if(play_sounds==1)
    	 fmod_stopeffect();
}

void let_the_music_play(int num)
{
      if(sound_p==true)
        fmod_playsound(num);
      else if(sound_p==false){
        fmod_stopsound();
          }
}

void goodbye(){
fmod_stopsound();
fmod_stopeffect();
}

