#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>//读写文件
#include<string.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>

#include "main.h"

int show_bmp(char const* name);

int show_hua(int t_x0);
int event_init();
int lcd_init();
int x0c();
int x0c1();
int music();



int gameball();
void* Move_Ball(void* arg);
int ban_hua();
int gameover();
void* Touch_Ctrl_Plate(void* arg);



int desktop();
void ballgame();

int x0, yy, j, n, m = 0;
char c;
char bmp[6] = "1.bmp";
int mflag = 0;
int mret = 0;
int spflg = 0;

int event_fd;
int lcd_fd;
int* p;

int q_x0 = 0;
int gover = 0;

//触摸屏初始化
int event_init()
{
    event_fd = open("/dev/input/event0", O_RDWR);
    if (event_fd == -1)
    {
        printf("打开触摸屏失败\n");
        return 0;
    }
}

//LCD初始化
int lcd_init()
{
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        printf("lcd error\n");
        return 0;
    }
}

int gameball()
{
    gover = 0;
    pthread_t P_ID;
    pthread_create(&P_ID, NULL, Move_Ball, NULL);
    pthread_t Q_ID;
    pthread_create(&Q_ID, NULL, Touch_Ctrl_Plate, NULL);
    while (1)
    {

        if (gover == 1)
        {
            
            show_bmp("goball.bmp");
            pthread_cancel(P_ID);
            pthread_cancel(Q_ID);
            gameover();
            break;
        }
    }
}

//球的运动
void* Move_Ball(void* arg)
{
    int i = 0, j = 0;
    for (j = 0; j < 480; j++)
    {
        for (i = 0; i < 800; i++)
        {
            p[j * 800 + i] = 0x000000000;
        }
    }

    int x0 = 400, yy = 240, r = 30, flag_x0 = 0, flag_yy = 0;

    while (1)
    {
        for (j = yy - r; j <= yy + r; j++)
        {
            for (i = x0 - r; i <= x0 + r; i++)
            {
                if ((i - x0) * (i - x0) + (j - yy) * (j - yy) < r * r)
                {
                    p[800 * j + i] = 0x000f1939c;//f1939c
                }
                else
                {
                    p[800 * j + i] = 0x000000000;
                }
            }
        }



        usleep(3000);
        if (yy - r == 0)//判断球碰到上面墙壁
            flag_yy = 1;
        if (yy + r == 400 && x0<q_x0 + 50 && x0>q_x0 - 50)//判断球碰到木板
            flag_yy = 0;
        if (yy + r > 400)
        {
            gover = 1;
            break;
        }
        if (x0 - r == 0)
            flag_x0 = 1;
        if (x0 + r == 800)
            flag_x0 = 0;

        if (flag_yy == 0)
            yy--;
        if (flag_yy == 1)
            yy++;
        if (flag_x0 == 0)
            x0--;
        if (flag_x0 == 1)
            x0++;
    }
    return NULL;
}

//木板滑动
int ban_hua()
{
    for (int yy = 400; yy < 430; yy++)
    {
        for (int x0 = 0; x0 < 800; x0++)
        {
            if (x0 >= q_x0 - 50 && x0 <= q_x0 + 50)//小木板
            {
                p[800 * yy + x0] = 0x0002e317c;
            }
            else//将木板经过或者没有经过的地方重新变回黑色
            {
                p[800 * yy + x0] = 0x000000000;
            }
        }
    }
    return 0;
}

//获取木板坐标
void* Touch_Ctrl_Plate(void* arg)
{
    //打开触摸屏设备文件
    int event_fd = open("/dev/input/event0", O_RDONLY);
    if (event_fd == -1)
    {
        printf("打开触摸屏失败\n");
        return NULL;
    }

    //存放触摸屏数据
    struct input_event ts;
    int x0 = 0, yy = 0;
    while (1)
    {
        read(event_fd, &ts, sizeof(ts));

        if (ts.type == EV_ABS && ts.code == ABS_X)
        {
            q_x0 = ts.value;
        }
        if (ts.type == EV_ABS && ts.code == ABS_Y)
        {
            yy = ts.value;
        }
        ban_hua();
    }
    return NULL;
}

//游戏结束
int gameover()
{
    show_bmp("goball.bmp");
    
    while(1)
    {
        project_touch();
        //压力值判断下
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)//
        {
            //4.为所欲为
            if(x>271&&x<346 && y>367&&y<441)
            {
                ballgame();
            }
            if(x>469&&x<551 && y>367&&y<442)
            {
                interface();
            }
        }
    }
}

int show_bmp(char const* name)
{
    //打开bmp图片
    int bmp_fd = open(name, O_RDWR);
    if (bmp_fd == -1)
    {
        printf("bmp error\n");
        return 0;
    }
    //跳过用来存储图片信息的54个字节
    lseek(bmp_fd, 54, SEEK_SET);
    //存储图片数据
    char buf[800 * 480 * 3];
    //读取图片全部数据
    read(bmp_fd, buf, 800 * 480 * 3);
    //保存塞入像素点后的图片数据
    int bmp[800 * 480];
    int i, j;
    //每个像素点塞一个字节
    for (i = 0; i < 800 * 480; i++)
    {
        bmp[i] = buf[i * 3] << 0 | buf[i * 3 + 1] << 8 | buf[i * 3 + 2] << 16;
    }

    int x0, yy;
    for (yy = 0; yy < 480; yy++)//优化算法，解决图片颠倒问题
    {
        for (x0 = 0; x0 < 800; x0++)
        {
            *(p + 800 * (479 - yy) + x0) = bmp[800 * yy + x0];
        }
    }

    close(lcd_fd);
    close(bmp_fd);

    return 0;
}

void ballgame()
{
   /* lcd_init();
    ball_init(bp);
    while (1)
    {
        for (int i = 0; i < 480; i++)
        {
            for (int j = 0; j < 800; j++)
            {
                point(j, i, Black);
            }
        }
        show_circle(bp->yy0, bp->x00, bp->r, Red);
        ball_move(bp);
    }
    lcd_destroyy();*/

    // 触摸屏初始化
    event_init();
    lcd_init();
    //内存映射
    p = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (p == NULL)
    {
        printf("内存映射失败\n");
    }
    gameball();

}