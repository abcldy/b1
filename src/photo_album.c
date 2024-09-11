#include"main.h"
#define NUM 3
char * arr_bmpname[5] = {"111.bmp","113.bmp","gameover.bmp"};//图片名
int i = 0;//记录此刻播放的图片
int j = 0;//记录此前播放的图片 
void ppt()
{
    

    // if(x > 700&& y < 20)
    // {
    // while (1)
    // 	{
    // 		show_1152000bmp(arr_bmpname[i],FB);
    // 		sleep(1);
            
    // 		if (4 == i)
    // 		{
    // 			i = 0;
    // 		}
    //         else i++;
    // 	}
    //     /*
    //        ppt放映：
            
    //     */
    // }
    
    // if(i==NUM)
    // {
    //     i=0;
    // }
    // else
    // {
    //     i++
    // };
    // show_1152000bmp(arr_bmpname[i],FB);
    // printf("i = %d\n",i);

    // if(i==-1)
    // {
    //     i=NUM-1;
    // }
    // else 
    // {
    //     i--;
    // }
    show_1152000bmp(arr_bmpname[i],FB);
    printf("i = %d\n",i);
    

}

int touchscreen()
{
	int x,y;//记录触摸坐标
    int _x;int _y;//记录前0.3s的触摸坐标
	
	while(1)
	{
		//2.读取文件内容
		read(touch_fd , &touch , sizeof(touch));
		//sizeof会自动测量结构体的字节数
	
		//3.筛选出需要的信息
		//判断是不是触摸屏
		if(touch.type == EV_ABS)//0x03
		{
			//判断是不是X
			if(touch.code == ABS_X)
			{
				x = touch.value*800/1024;
				//x = touch.value;
			}
		}
		
		if(touch.type == EV_ABS)//0x03
		{
			//判断是不是y
			if(touch.code == ABS_Y)
			{
				y = touch.value*480/600;
				//y = touch.value;
			}
		}
		
		//判断是不是按键事件
		if(touch.type == EV_KEY)
		{
			//判断是不是触摸事件
			if(touch.code == BTN_TOUCH)
			{
				if(touch.value == 0)
				{
                    if(_x - x > 30)
                    {
                        i++;
                    }
                    else if(_x - x < -30)
                    {
                        i--;
                    }
                    else
                    {
                        if(x <= 400)
                        {
                            i--;
                        }
                        else
                        {
                            i++;
                        }
                    }

				}
			}
		}
        _x = x; _y = y;

        if(i == -1){i = NUM - 1;}
        if(i == 5){i = 0;}

        j = i;
        usleep(300000);//延时0.3s
        if(x <= 40 && y <= 40)
        {
            printf("退出循环\n");
            break;
        }
        if(i == j){continue;}
        printf("i = %d\n",i);
        printf("j = %d\n",j);
        j = i;
	}
	  
}

int photo_album()
{
    touchscreen();
    printf("退出相册\n");
    return 0;
}