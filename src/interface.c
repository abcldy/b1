#include "main.h"

int interface()
{
    //主界面图片
    show_1152000bmp("interface.bmp", FB);

     while (1)
    {
        //触摸屏函数
        project_touch();
        //压力值判断下
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)//
        {
            //4.为所欲为
            printf("(%d,%d)\n",x,y);

            //电子相册
            if(x > 0 && x < 200 && y > 0 && y < 480)
            {
                
            }

            
            //音乐播放器
            if(x > 200 && x < 400 && y > 0 && y < 480)
            {
                
            }

            
            //视频播放器
            if(x > 400 && x < 600 && y > 0 && y < 480)
            {
               
            }

            
            //游戏
            if(x > 600 && x < 800 && y > 0 && y < 480)
            {
               
            }
            
        }
    }
    
    return 0;
}

}




