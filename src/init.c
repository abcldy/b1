#include "main.h"

/*项目初始化*/
int project_init()
{
    //LCD文件初始化
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1)
    {
        printf("LCD文件打开失败\n");
        return -1;
    }
    printf("LCD文件打开完成\n");

    //触摸屏文件初始化
    touch_fd = open("/dev/input/event0", O_RDONLY);
    if(touch_fd == -1)
    {
        printf("打开触摸屏文件失败\n");
        return -1;
    }
    printf("打开触摸屏文件完成\n");

    //内存映射初始化
    FB = mmap( NULL , 800*480*4 , PROT_READ | PROT_WRITE, MAP_SHARED,lcd_fd, 0);

    return 0;
}

/*项目释放*/
int project_free()
{
    //解除内存映射
    munmap(FB, 800*480*4);

    //关闭lcd文件
    close(lcd_fd);

    //关闭触摸屏文件
    close(touch_fd);

    return 0;
}