#include "ZenaidaG.h"
extern bool sound_p;
#ifdef USE_SOUND
extern int play_sounds;
#endif

void init_sounds(void)
{
    char file[] = "./sounds/zfc.mp3";
#ifdef USE_SOUND
        if(fmod_init()){
                printf("ERROR-fmod_init()\n\n");
                return;
        }
        if(fmod_createsound(file,0)){
                printf("ERROR-fmod_createsound()\n\n");
                return;
        }

        fmod_setmode(0,FMOD_LOOP_OFF);
#endif
}

void let_the_music_play()
{
    int k = 0;
      if(sound_p==true)
        fmod_playsound(k);
      else if(sound_p==false){
          //# ifdef USE_SOUND
        fmod_stopsound();
          //#endif
          }
}

