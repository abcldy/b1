#ifndef _MAIN_H_
#define _MAIN_H_
/*�ж�ͷ�ļ��Ƿ��ظ�����*/

/*ͷ�ļ�*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include "show_bmp.h"

//s
/*ȫ�ֱ���*/
int lcd_fd;//LCD�ļ�������
int x, y;//����������
int touch_fd;//�������ļ�������
int* FB;//�ڴ�ӳ��ָ��

/*�궨��*/

/*�ṹ��ͽṹ�嶨��*/
struct input_event touch;//������

///*��������*/
//int project_init();/*��Ŀ��ʼ��*/
//int project_show();/*�����溯��*/
//int project_free();/*��Ŀ�ͷ�*/
//void project_touch();/*����������*/
//void show_bmp();/*�ֶ��л�*/
//int project_music();/*���ֺ���*/
//int project_video();/*��Ƶ����*/
//int video_inti();/*�ܵ��ļ�����*/


int project_init();/*��Ŀ��ʼ��*/
int project_free();/*��Ŀ�ͷ�*/



/*ö�٣�������*/

#endif //�����������