#include "main.h"


// int x,y;
// void xy()//消抖部分
// {
//     int flag = 0;//获得的坐标的数量
//     while(1)
//     {
//         read(touch_fd , &touch , sizeof(touch));
//         if (touch.type == EV_ABS && touch.code == ABS_X && flag == 0) {x = touch.value*800/1024;flag = 1;}
//         if (touch.type == EV_ABS && touch.code == ABS_Y && flag == 1) {y = touch.value*480/600;flag = 2;}
//         if(flag == 2){flag = 0;break;}
       
//     }
// }


/*
    https://note.youdao.com/ynoteshare/index.html?id=26ba01a2fd26818b1900c2c75be56407&type=note&_time=1725863750164
*/

#define FIFO_PATH "/tmp/myFifo"



int send_cmd_music(  int fd_fifo , const char * cmd )
{

    int ret_val = write(fd_fifo , cmd , strlen(cmd) );
    if (-1 == ret_val)
    {
        perror("指令发送失败....\n");
        return -1 ;
    }
    
    printf("指令发送成功%d\n" , ret_val);
    
    return 0 ;
}


int music1()
{
    int  ret_val ;
    // 创建用于发送控制指令的管道文件
    if (access( FIFO_PATH , F_OK )) // 判断管道文件是否已存在
    {
        ret_val = mkfifo(FIFO_PATH, 0666 );
        if (ret_val < 0 )
        {
            perror("mkfifo error");
            return -1 ;
        }
        printf("管道文件创建成功\n");
    }

    // 直接使用 popen 来播放音视频
    FILE * fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
    // FILE * fp = popen("/usr/bin/madplay music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
    /*
        /dev/mplayer  3.avi  -geometry 100:200 -zoom -x 400 -y 200 &
        -geometry 100:200   设置视频播放的位置 
        -zoom -x 400 -y 200  设置视频的宽度和高度
    */

    if ( fp == NULL )
    {
        perror("popen error");
        return -1;
    }
    

    // printf("音频正在播放....\n");

    // 打开用于发送指令的管道文件
    int fd_fifo = open(FIFO_PATH , O_RDWR );
    if (-1 == fd_fifo)
    {
        perror("open fifo error");
        return -1;
    }
    

    /*
        1.播放2.暂停/继续3.下一个视频4.结束5.快进5s
        6.退回5s
    */
    int i=0;//输入
    //int n = 0;//播放/暂停
    int m = 0;//视频选择
    show_1152000bmp("music2.bmp", FB);
 
    while(1)//开始输入命令
    {
        // printf("1.播放 2.暂停/继续 3.下一个视频 4.退出 5.快进5s 6.退回5s \n");
        
        
        // project_touch();
        // if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        // {

        // xy();
        project_touch();
        //  if(x>0&&y>0)
        //     {
        //     sleep(1);
        //     break;
        //     }
        if(y>360&y<400)
        {
            if(x>250&x<285)
            {i=6;}//前进10
            if(x>320&x<355)
            {i=1;}//播放
            if(x>360&x<415)
            {i=2;}//暂停
            if(x>450&x<485)
            {i=5;}//倒退10
            if(x>560&x<590)
            {i=3;}//切换

        } 
        // if (x>=700 && x<=800 && y>=0 && y<=100)//退出
        // {
        //     printf("退出\n");
           
        //     break;
        // }
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {
            if (x>=700 && x<=800 && y>=0 && y<=100)//退出
            {
                printf("退出\n");
                sleep(1);
                interface();
                break;
            }
 
        }
        // if(x>720&y<50)
        //     {
        //         i=4;
        //     }
        // if(x>770&y<50)
        // {
        // system("killall -9 mplayer");
        //     interface();
        //     break;
        // }
        //xy();
        printf("%d,%d\n",x,y);
            
            
            // xy();
            // printf("%d,%d\n",x,y);
            // // if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
            // // {
            // if(x > 643 && x < 662 && y > 201 && y < 220)
            // {
            //     send_cmd_music(fd_fifo,"seek -5\n");//后退5秒
            // }
            // // if(x > 97 && x < 112 && y > 284 && y < 298)
            // // {
            
            // // }
            // if(x > 745 && x < 764 && y > 201 && y < 220)
            // {
            //     send_cmd_music(fd_fifo,"seek +5\n");//前进5秒
            // }
            // if(x > 688 && x < 719 && y > 169 && y < 226)
            // {
            //     send_cmd_music(fd_fifo,"pause\n");//暂停
            // }

            // if(x > 644 && x < 678 && y > 380 && y < 410)
            // {
            //     send_cmd_music(fd_fifo,"volume -10\n");//音量-10
            // }
            // if(x > 740 && x <772 && y > 348 && y < 410)
            // {
            //     send_cmd_music(fd_fifo,"volume +10\n");//音量+10
            // }
            // if(x > 770 && x < 800 && y > 0 && y < 30)
            // {
            //     system("killall -9 mplayer");
            //     interface();
            //     break;
            // }
    
           

        // if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        // {   
            
        //     if(y>360&y<400)
        //     {
        //         if(x>250&x<285)
        //         {
                
        //             send_cmd_music( fd_fifo , "seek -10\n" );//退10秒
                    
        //         }
        //         if(x>320&x<355)
        //         {
        //             fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");//播放
        //         }
        //        if(x>380&x<415)
        //         {
        //          send_cmd_music( fd_fifo , "pause\n" );//暂停   
        //         }

        //         if(x>450&x<485)
        //         {
        //           send_cmd_music( fd_fifo , "seek +10\n" );  //前进10秒
        //         }

        //     }
        //     else
        //     {
        //       if(x>770&y<50)
        //         {
        //         system("killall -9 mplayer");
        //             interface();
        //             break;
        //         }  

        //     }
   
        switch (i)
        {      
            case 1:
            fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
            // fp = popen("/usr/bin/madplay music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
            break;
            case 2:
            send_cmd_music( fd_fifo , "pause\n" );//暂停
            break;
            case 3:
            if(m == 0)
            {
                system("killall -9 /dev/mplayer");//结束播放器
                fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                // fp = popen("/usr/bin/madplay music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 1;
            }
            else
            {
                system("killall -9 /dev/mplayer");//结束播放器
                fp = popen("/dev/mplayer music(1).mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                // fp = popen("/usr/bin/madplay music(1).mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 0;
            }
            break;
            case 4:
            system("killall -9 /dev/dsp");
            interface();
            break;
            // printf("%d",&x);
            // send_cmd_music( fd_fifo , "quit\n" );

            // break;
            case 5:
            send_cmd_music( fd_fifo , "seek +10\n" );
            break;
            case 6:
            send_cmd_music( fd_fifo , "seek -10\n" );
            break;
            
            default:
            break;
            i = 0;
        }
        

        //     //sleep(1);
        // }

    }//while
    interface();
    pause();
    return 0;
}


