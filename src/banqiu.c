#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>//读写文件
#include<string.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>

int show_bmp(char const* name);
int event_xy(int* x, int* y);
int show_hua(int t_x);
int event_init();
int lcd_init();
int xc();
int xc1();
int music();
int musicplay();
int sp();
int sp1();
int gameball();
void* Move_Ball(void* arg);
int ban_hua();
int gameover();
void* Touch_Ctrl_Plate(void* arg);
int myregister();
int login();
int login1();
int desktop();
void ballgame();

int x, y, j, n, m = 0;
char c;
char bmp[6] = "1.bmp";
int mflag = 0;
int mret = 0;
int spflg = 0;

int event_fd;
int lcd_fd;
int* p;

int q_x = 0;
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
            show_bmp("gameover_ball.bmp");
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
            p[j * 800 + i] = 0x00000000;
        }
    }

    int x = 400, y = 240, r = 50, flag_x = 0, flag_y = 0;

    while (1)
    {
        for (j = y - r; j <= y + r; j++)
        {
            for (i = x - r; i <= x + r; i++)
            {
                if ((i - x) * (i - x) + (j - y) * (j - y) < r * r)
                {
                    p[800 * j + i] = 0x000000ff;
                }
                else
                {
                    p[800 * j + i] = 0x00000000;
                }
            }
        }



        usleep(3000);
        if (y - r == 0)//判断球碰到上面墙壁
            flag_y = 1;
        if (y + r == 400 && x<q_x + 50 && x>q_x - 50)//判断球碰到木板
            flag_y = 0;
        if (y + r > 400)
        {
            gover = 1;
            break;
        }
        if (x - r == 0)
            flag_x = 1;
        if (x + r == 800)
            flag_x = 0;

        if (flag_y == 0)
            y--;
        if (flag_y == 1)
            y++;
        if (flag_x == 0)
            x--;
        if (flag_x == 1)
            x++;
    }
    return NULL;
}

//木板滑动
int ban_hua()
{
    for (int y = 400; y < 430; y++)
    {
        for (int x = 0; x < 800; x++)
        {
            if (x >= q_x - 50 && x <= q_x + 50)//小木板
            {
                p[800 * y + x] = 0x00ff0000;
            }
            else//将木板经过或者没有经过的地方重新变回黑色
            {
                p[800 * y + x] = 0x00000000;
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
    int x = 0, y = 0;
    while (1)
    {
        read(event_fd, &ts, sizeof(ts));

        if (ts.type == EV_ABS && ts.code == ABS_X)
        {
            q_x = ts.value;
        }
        if (ts.type == EV_ABS && ts.code == ABS_Y)
        {
            y = ts.value;
        }
        ban_hua();
    }
    return NULL;
}

//游戏结束
int gameover()
{
    show_bmp("gameover_ball.bmp");
    
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

    int x, y;
    for (y = 0; y < 480; y++)//优化算法，解决图片颠倒问题
    {
        for (x = 0; x < 800; x++)
        {
            *(p + 800 * (479 - y) + x) = bmp[800 * y + x];
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
        show_circle(bp->y0, bp->x0, bp->r, Red);
        ball_move(bp);
    }
    lcd_destroy();*/

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