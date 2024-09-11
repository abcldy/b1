#ifndef _MAIN_H_
#define _MAIN_H_
/*判断头文件是否被重复包含*/

/*头文件*/
#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include"stdlib.h"
#include <string.h>

/*全局变量*/
int lcd_fd;//LCD文件描述符
int x,y;//触摸屏坐标
int touch_fd;//触摸屏文件描述符
int *FB;//内存映射指针
//char * arr_bmpname[5] = {"111.bmp","112.bmp","113.bmp","bp.bmp","../b1.bmp"};//图片名


/*宏定义*/

/*结构体和结构体定义*/
struct input_event touch;//触摸屏

/*函数声明*/
int project_init();/*项目初始化*/
int project_free();/*项目释放*/
int interface();/*主机面函数*/
int show_location_bmp(char *pathname,int lcd_x_coordinates, int lcd_y_coordinates,int *show_bmp_lcd_mmap);/*图片展示*/
void project_touch();/*触摸屏函数*/
int flybird();/*game2*/
void game();
int show_1152000bmp();
void ballgame();

void music1();//音乐函数
int video();
// int project_music();/*音乐函数*/
int project_video();/*视频函数*/
// int video_inti();/*管道文件函数*/

/*枚举，联合体*/

#endif //条件编译结束