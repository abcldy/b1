#include "main.h"

void game()
{
	// project_init();
    show_1152000bmp("kaishi.bmp", FB); //写入游戏开始界面
	while(1){
    
    project_touch();
    if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
    {
        ballgame();
    // if(1)
    // {
    //     //banqiu.c
    // }
    // if(x>233*800/1024&&x<614*800/1024 && y>253*480/600&&y<340*480/600)
    // {
    //     flybird();//flybird.c
    // }
    // else
    // {
    //     break;
    // }

    }
    }
}

