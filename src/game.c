#include "main.h"

void game()
{
	// project_init();
    show_1152000bmp("kaishi.bmp", FB); //写入游戏开始界面
	while(1){
    
    project_touch();
    if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
    {
        // printf("(%d,%d)\n",x,y);
        
    if(x>221&&x<600 && y>112&&y<192)
    {
        ballgame();
    }
    if(x>233&&x<614 && y>253&&y<340)
    {
        flybird();//flybird.c
    }
    // else
    // {
    //     break;
    // }

    }
    }
}

