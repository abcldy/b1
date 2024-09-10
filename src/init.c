#include "main.h"

/*��Ŀ��ʼ��*/
int project_init()
{
    //LCD�ļ���ʼ��
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        printf("LCD�ļ���ʧ��\n");
        return -1;
    }
    printf("LCD�ļ������\n");

    //�������ļ���ʼ��
    touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd == -1)
    {
        printf("�򿪴������ļ�ʧ��\n");
        return -1;
    }
    printf("�򿪴������ļ����\n");

    //�ڴ�ӳ���ʼ��
    FB = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED,
        lcd_fd, 0);

    return 0;
}

/*��Ŀ�ͷ�*/
int project_free()
{
    //����ڴ�ӳ��
    munmap(FB, 800 * 480 * 4);

    //�ر�lcd�ļ�
    close(lcd_fd);

    //�رմ������ļ�
    close(touch_fd);

    return 0;
}