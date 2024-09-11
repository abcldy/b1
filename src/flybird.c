#define USE_ARM 1
#define USE_DBG	1

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include "math.h"

#if USE_ARM
#include <linux/input.h>
#include <unistd.h>
#include <sys/mman.h>
#endif

//#define SPACE_HEIGHT	100
#define WALL_WIDTH		60
#define BIRD_WIDTH		68
#define BIRD_HEIGHT		50
#define dbgprintf	//printf("%d\t%s\n",__LINE__,__FUNCTION__);
int*plcd;
int bird_score = 0;
char pbackground[800*480*3];
char bird_up_color[BIRD_HEIGHT*BIRD_WIDTH*3];
char bird_down_color[BIRD_HEIGHT*BIRD_WIDTH*3];
int touch_down = 0;
int bird_x = 60,bird_y = 200;
int game_over = 0;

#if USE_ARM

int bird_up_getpoint(int x,int y)
{
	int color = 0;;
	color =	((*(bird_up_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 0)<<0) 
		| 	 (*(bird_up_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 1)<<8) 
		|	 (*(bird_up_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 2)<<16));
	return color;
}
int bird_down_getpoint(int x,int y)
{
	int color = 0;;
	color =	((*(bird_down_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 0)<<0) 
		| 	 (*(bird_down_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 1)<<8) 
		|	 (*(bird_down_color + (BIRD_HEIGHT-1-y)*BIRD_WIDTH*3+(x)*3 + 2)<<16));
	return color;
}
int lcd_getpoint(int x,int y)
{
	int color = 0;;
	color =	((*(pbackground + (479-y)*800*3+(x)*3 + 0)<<0) 
		| 	 (*(pbackground + (479-y)*800*3+(x)*3 + 1)<<8) 
		|  	 (*(pbackground + (479-y)*800*3+(x)*3 + 2)<<16));
	return color;
}

void lcd_drawpoint(int x,int y,int color)
{
	if(x >= 0&& x<800 && y>= 0 && y < 480 )
		*(plcd + y*800 + x) = color;
}

void lcd_fill(int color)
{
	int x,y;
	for(y =0;y<480;y++)
	{
		for(x =0;x<800;x++)
		{
			lcd_drawpoint(x,y,color);
		}
	}
}

void ShowBmp(int x0,int y0,char* path)
{
	int ret,fd_bmp;
	int width,height,deep;
	char buf[4];
	fd_bmp = open(path,O_RDONLY);
	if(fd_bmp < 0)
	{
		perror("open bmp error:");
		return;
	}
	lseek(fd_bmp,0,SEEK_SET);
	ret = read(fd_bmp,buf,2);
	if(ret < 0)
	{
		perror("read bmp error:");
		return;
	}
	//判断是否为bmp文件
	if(buf[0] == 0x42 && buf[1] == 0x4D)
	{
		//printf("\nthis file is .bmp.\n");
	}
	else
	{
		printf("\nthis file isn't .bmp.\n");
		return;
	}
	
	lseek(fd_bmp,0X12,SEEK_SET);
	ret = read(fd_bmp,buf,4);
	width = buf[0]<<0 | buf[1]<<8 | buf[2]<<16 | buf[3]<<24;
	//printf("ret:%x,width:%d\n",ret,width);
	
	lseek(fd_bmp,0X16,SEEK_SET);
	ret = read(fd_bmp,buf,4);
	height = buf[0]<<0 | buf[1]<<8 | buf[2]<<16 | buf[3]<<24;
	//printf("ret:%x,height:%d\n",ret,height);
	
	lseek(fd_bmp,0X1C,SEEK_SET);
	ret = read(fd_bmp,buf,2);
	deep = buf[0]<<0 | buf[1]<<8;
	//printf("ret:%x,deep:%d\n",ret,deep);
	
	int bytes_per_line = abs(width)*(deep/8);//每行的字节数
	int laizi = 0;
	if(bytes_per_line % 4 != 0)
	{
		laizi = 4 - bytes_per_line%4;
	}
	bytes_per_line += laizi;
	int size = bytes_per_line * abs(height);
	char *bmpbuf = (char*)malloc(size);
	lseek(fd_bmp,0X36,SEEK_SET);
	ret = read(fd_bmp,bmpbuf,size);
	int i = 0,x,y,color;
	int disapper_color = 0x00000000;
	int disapper_enable = 0;
	unsigned char r,g,b,a = 0;
	disapper_color = (bmpbuf[0]<<0 | bmpbuf[1]<<8 | bmpbuf[2]<<16);
	if(disapper_color == 0x00ff0000)
		disapper_enable = 1;
	for(y = 0;y < abs(height);y++)
	{
		for(x= 0;x<abs(width);x++)
		{
			b = bmpbuf[i++];
			g = bmpbuf[i++];
			r = bmpbuf[i++];
			if(deep == 32)
			{
				a = bmpbuf[i++];
			}
			else
			{
				a = 0x00;
			}
			color = (b<<0)|//b
					(g<<8)|//g
					(r<<16)|
					(a<<24);
			if(disapper_enable == 1 && color == disapper_color)
				lcd_drawpoint(x + x0,(abs(height) - y + y0),lcd_getpoint(x + x0,(abs(height) - y + y0)));
			else
				lcd_drawpoint(x + x0,(abs(height) - y + y0),color);
		}
		i += laizi;
	}
	free(bmpbuf);
	close(fd_bmp);
	return;
}

void ShowNum(int x0,int y0,int num,int len)
{
	
	dbgprintf;
	char path[60];
	int  i =0;
	while(num)
	{
		dbgprintf;
		sprintf(path,"gpbn/%d.bmp",num%10);//green_paper_blue_number
		dbgprintf;
		ShowBmp(700 - i*16,100,path);
		dbgprintf;
		++i;
		num /= 10;
	}
}


#endif


void* touch_task()
{
		//打开
	int fd_touch = open("/dev/input/event0",O_RDWR);
	if(fd_touch < 0)
	{
		printf("open event0 error\n");
		return 0;
	}
	//读数据,要从触摸屏读一个输入事件
	int x1 = -1,y1 = -1;
	int x2,y2;
	struct input_event ev;
		
	while(1)
	{
		int ret = read(fd_touch,&ev,sizeof(ev));
		if(ret != sizeof(ev))
		{
			continue;
		}
		//x输入
		if(ev.type == EV_ABS && ev.code ==ABS_X)
		{
			if(x1 == -1)
			{
				x1 = ev.value;
			}
			x2 = ev.value;
		}
		if(ev.type = EV_ABS && ev.code == ABS_Y)
		{
			if(y1 == -1)
			{
				y1 = ev.value;
			}
			y2 = ev.value;
		}
		if(ev.type = EV_ABS && ev.code == BTN_TOUCH && ev.value == 0)
		{
			touch_down = 0;//松开了
			if(x2 > 700 && y2 < 100)
			{
				game_over = 0;
			}
			x1 = y1 = -1;
		}
		if(ev.type = EV_ABS && ev.code == BTN_TOUCH && ev.value == 1)
		{
			touch_down = 1;//一直在按着
		}
		//printf("%d\n",touch_down);
	}
	//关闭
	close(fd_touch);
}


void* build_one_wall()
{
	int x,y;
	int space_height = rand()%50 + 150 - bird_score*2;
	int space_poisition = rand()%(480-space_height);
	int up_color = rand()%0x00ffffff;
	int down_color = rand()%0x00ffffff;
	pthread_detach(pthread_self());
	for(x = 800-1;x>=0-WALL_WIDTH;x--)
	{
		if(!game_over)
		{
			for(y = 0;y<space_poisition;y++)
			{
				lcd_drawpoint(x,y,				up_color);//画点0x0000ff00
				lcd_drawpoint(x+WALL_WIDTH,y,lcd_getpoint(x+WALL_WIDTH,y));//0x000000ff);//擦除
			}
			for(y = space_poisition+space_height;y<480;y++)
			{
				lcd_drawpoint(x,y,				down_color);//画点0x0000ffff
				lcd_drawpoint(x+WALL_WIDTH,y, lcd_getpoint(x+WALL_WIDTH,y));//擦除
			}
			if(x <= bird_x + BIRD_WIDTH -2 && x+WALL_WIDTH > bird_x +2)
			{
				if(bird_y + 2<space_poisition || bird_y + BIRD_HEIGHT -2 > space_poisition + space_height)
				{
					game_over = 1;
                    printf("game over!!!\n");
				}
			}
			usleep(3000);//控制柱子移动速度
		}
	}
	bird_score++;
}


void* build_sone_wall()
{
	int i = 0;
	pthread_t mythread;
	while(1)//每隔一段时间产生一个柱子
	{
		if(!game_over)
			pthread_create(&mythread,NULL,build_one_wall,NULL);
		sleep(2);//产生柱子的速度
	}
}

void*score_task()
{
	while(1)
	{
		printf("分数为：%d\n",bird_score);
		ShowNum(700,100,bird_score,4);
		sleep(1);
	}
}
void savebirdbmp(char* path,char*bmpbuf)//读取到数组
{
	int bmp = open(path,O_RDONLY);
	if(bmp < 0)
	{
		perror("open bmp.bmp:");
		return ;
	}
	lseek(bmp,SEEK_SET,0x36);//0x36
	read(bmp,bmpbuf,BIRD_HEIGHT*BIRD_WIDTH*3);
	close(bmp);
}
void ShowBGArray(int x0,int y0)
{
	int x,y;
	dbgprintf;
	for(y =0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
				lcd_drawpoint(x0 + x,y0+y,lcd_getpoint(x,y));
		}
	}
	dbgprintf;
}


void ShowBirdArray(int x0,int y0,int up)
{
	int x,y;
	dbgprintf;
	for(y =0;y<BIRD_HEIGHT;y++)
	{
		for(x=0;x<BIRD_WIDTH;x++)
		{
			if(up == 1)
				lcd_drawpoint(x0 + x,y0+y,bird_up_getpoint(x,y));
			else
				lcd_drawpoint(x0 + x,y0+y,bird_down_getpoint(x,y));
		}
	}
	dbgprintf;
}


void birdfly()
{
	int x0;
	static int up_cnt = 0;
	static int down_cnt = 0;
	int bird_color = rand()%0x00ffffff;
	//int bird_color = 0x00ffffff/34950*bird_y;
	//int bird_color = (bird_score*100)%0x00ffffff;
	if(!game_over)
	{
		if(touch_down == 1)//按下，y减少
		{
			down_cnt = 0;
			up_cnt += 20;
			if(up_cnt >= 3000)
				up_cnt = 3000;
			//ShowBmp(bird_x,bird_y,"./bmp/flybird/bird_up.bmp");
			ShowBirdArray(bird_x,bird_y,1);
			for(x0 = bird_x;x0<bird_x + BIRD_WIDTH;x0++)
			{
				//lcd_drawpoint(x0,bird_y,		bird_color);//画鸟，方块鸟
	//			lcd_drawpoint(x,y+50,	0x000000ff);
				lcd_drawpoint(x0,bird_y+BIRD_HEIGHT,lcd_getpoint(x0,bird_y + BIRD_HEIGHT));//擦除

			}

			if(bird_y>0)
				bird_y--;
			usleep(3500-up_cnt);
		}
		else
		{
			up_cnt = 0;
			down_cnt += 20;
			if(down_cnt >= 3000)
				down_cnt = 3000;
			//ShowBmp(bird_x,bird_y ,"./bmp/flybird/bird_down.bmp");
			ShowBirdArray(bird_x,bird_y,0);
			for(x0 = bird_x;x0<bird_x + BIRD_WIDTH;x0++)
			{
				//lcd_drawpoint(x0,bird_y+BIRD_HEIGHT,bird_color);画鸟
				//lcd_drawpoint(x,y,	0x000000ff);
				lcd_drawpoint(x0,bird_y,lcd_getpoint(x0,bird_y));//擦除
			}

			if(bird_y<479-BIRD_HEIGHT)
			{
				bird_y++;
			}
			else
				;//game_over = 1;
			usleep(3500 - down_cnt);//延时减少，速度加快
		}
	}
}



int flybird(int argc,char* argv[])
{
	int i,j,x,y;
	srand(time(0));
#if USE_ARM
	int fb0 = open("/dev/fb0",O_RDWR);//触摸屏
	if(fb0 < 0)
	{
		perror("fb0:");
		return -1;
	}
	plcd = mmap(NULL, 480*800*4, PROT_WRITE | PROT_READ, MAP_SHARED,fb0, 0);
	lcd_fill(0x000000ff);
	ShowBmp(0,0,"background.bmp");
	int background = open("background.bmp",O_RDONLY);
	if(background < 0)
	{
		perror("open background.bmp:");
		return -1;
	}
	lseek(background,SEEK_SET,0x36);//0x36
	read(background,pbackground,800*480*3);
	close(background);
	//ShowBGArray(0,0);
	savebirdbmp("bird_up.bmp",bird_up_color);
	savebirdbmp("bird_down.bmp",bird_down_color);
	ShowBirdArray(50,100,1);
	ShowBirdArray(150,400,0);

#endif
	pthread_t touch_task_pthread,build_pthread,score_pthread;
	pthread_create(&touch_task_pthread,NULL,touch_task,NULL);
	pthread_create(&build_pthread,NULL,build_sone_wall,NULL);
	pthread_create(&score_pthread,NULL,score_task,NULL);
	//pthread_create(&pthread2,NULL,f2,NULL);
	//pthread_create(&pthread3,NULL,f3,NULL);
	usleep(100);
	dbgprintf;
	while(1)
	{
		birdfly();
		if(game_over)
		{
			ShowBmp(0,0,"gameover.bmp");
			while(game_over);
			bird_y = 100;
			bird_score = 0;
			ShowBmp(0,0,"background.bmp");
		}
	}
	close(fb0);
	return 0;
}

















