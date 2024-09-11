#include "main.h"

int game()
{
	int x,y;
	project_init();
	
	show_1152000bmp("kaishi.bmp", FB); //写入游戏开始界面
	project_touch();
    if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
    {
        printf("yyyyy");
    //     if(1)
    // {
    //     //banqiu.c
    // }
    // if(2)
    // {
    //     flybird();//flybird.c
    // }
    }
    
	return 0;
}

