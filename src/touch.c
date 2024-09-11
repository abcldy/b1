#include "main.h"

void project_touch()
{
    read(touch_fd , &touch , sizeof(touch));
        //printf("读取触摸屏文件完成\n");

    if (touch.type == EV_ABS && touch.code == ABS_X) x = touch.value*800/1024;//黑板
    if (touch.type == EV_ABS && touch.code == ABS_Y) y = touch.value*480/600;//黑板
    // if (touch.type == EV_ABS && touch.code == ABS_X) x = touch.value;//
    // if (touch.type == EV_ABS && touch.code == ABS_Y) y = touch.value;//
}