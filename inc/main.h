#ifndef _MAIN_H_
#define _MAIN_H_
/*判断头文件是否被重复包含*/

/*头文件*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include "show_bmp.h"

/*全局变量*/
int lcd_fd;//LCD文件描述符
int x, y;//触摸屏坐标
int touch_fd;//触摸屏文件描述符
int* FB;//内存映射指针

/*宏定义*/

/*结构体和结构体定义*/
struct input_event touch;//触摸屏

///*函数声明*/
//int project_init();/*项目初始化*/
//int project_show();/*主机面函数*/
//int project_free();/*项目释放*/
//void project_touch();/*触摸屏函数*/
//void show_bmp();/*手动切换*/
//int project_music();/*音乐函数*/
//int project_video();/*视频函数*/
//int video_inti();/*管道文件函数*/


int project_init();/*项目初始化*/
int project_free();/*项目释放*/



/*枚举，联合体*/

#endif //条件编译结束