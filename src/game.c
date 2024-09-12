#include "main.h"

void game()
{
	// project_init();
    show_1152000bmp("kaishi.bmp", FB); //写入游戏开始界面
	while(1){
    
    project_touch();
    if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
    {

    if(x>32&&x<118 && y>100&&y<151)
    {
        ballgame();
    }
    if(x>27&&x<123 && y>207&&y<264)
    {
        flybird();//flybird.c
    }
   

    }
    }
}

