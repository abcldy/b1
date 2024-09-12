#include "main.h"

void jinru(){
    show_1152000bmp("jinru.bmp", FB);
     while(1)
    {
        //触摸屏函数
        project_touch();
        //压力值判断下
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {
            if(x>0&&y>0)
            {sleep(1);
            interface();
            break;}
        }
    }

}
