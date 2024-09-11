#include "main.h"


#include "pm.h"
#include "ev.h"
#include <stdio.h>
 
 
 
int game()
{
	int x,y;
	project_init();
	
	show_1152000bmp("kaishi.bmp", FB); //写入游戏开始界面
	Get_ev(&x,&y);
	return 0;
}

