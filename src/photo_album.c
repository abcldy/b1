#include"main.h"
#define NUM 3
char * arr_bmpname[NUM] = {"goball.bmp","111.bmp","gameover.bmp"};//图片名
int i = 0;//记录此刻播放的图片

typedef struct photo_hub//图片名结构体
{
    int i;
    char*bmp_name;
    struct photo_hub* next;
}ph;


// ph* photo_list(char*name[NUM][20])
// {
//     for(int i = NUM -1;i>=0;i--)
//     {
//         (ph*)node = (ph*)malloc(sizeof(ph));//当前节点
//         node->i = i;
//         node->bmp_name = name[i];
//         node->next = NULL;

//         i--;
//         (ph*)node = (ph*)malloc(sizeof(ph));//下一节点
//         node->i = i;
//         node->bmp_name = name[i];
//         node->next = NULL;


//     }
// }

int getslide()
{
    int flag = 0;//获得的坐标的数量
    int x1,y1,x2,y2;
    while(1)
    {
        read(touch_fd , &touch , sizeof(touch));
        if (touch.type == EV_ABS && touch.code == ABS_X && flag == 0) {x1 = touch.value*800/1024;flag = 1;}
        if (touch.type == EV_ABS && touch.code == ABS_Y && flag == 1) {y1 = touch.value*480/600;flag = 2;}
        if(flag == 2){flag = 0;break;}

    }
    x2 = x1;y2 = y1;
    while(1)
    {
        read(touch_fd , &touch , sizeof(touch));
        if (touch.type == EV_ABS && touch.code == ABS_X) {x2 = touch.value*800/1024;}
        if (touch.type == EV_ABS && touch.code == ABS_Y) {y2 = touch.value*480/600;}    
        if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {
            //压力值为0的时候此时获取终点坐标
            break;	
        }	
    
    }

    if(x2 > x1 && x2*x2 - x1*x1 > y2*y2 - y1*y1){return 1;}//右划
    if(x2 < x1 && x1*x1 - x2*x2 > y1*y1 - y2*y2){return 2;}//左划
    if(y2 < y1 && y1*y1 - y2*y2 > x1*x1 - x2*x2){return 3;}//上划
    if(y2 > y1 && y2*y2 - y1*y1 > x2*x2 - x1*x1){return 4;}//下划
    if(x2 == x1 && y2 == y1){return 5;}//点击
}


int touchscreen()
{

	
	while(1)
	{
        int list = getslide();//记录滑动的类型
        if(list == 5)
        {
            project_touch();
            if(x <= 400){i--;}
            if(x >= 400){i++;}
        }
        if(list == 1){i--;}
        if(list == 2){i++;}
        if(list == 3){break;}


        if(i == -1){i = NUM - 1;}
        if(i == NUM){i = 0;}

        
        //usleep(3000);//延迟1ms
        // if(x <= 40 && y <= 40)
        // {
        //     printf("退出循环\n");
        //     break;
        // }
        show_1152000bmp(arr_bmpname[i],FB);
        printf("i = %d\n",i);   

    }
	  
}

int photo_album()
{
    show_1152000bmp(arr_bmpname[0], FB);//显示第一张图片
    touchscreen();
    printf("out of album\n");
    show_1152000bmp("interface.bmp", FB);//回到主界面
    return 0;
}