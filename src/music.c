#include"main.h"


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

    /*
        /dev/mplayer  3.avi  -geometry 100:200 -zoom -x 400 -y 200 &
        -geometry 100:200   设置视频播放的位置 
        -zoom -x 400 -y 200  设置视频的宽度和高度
    */

    if ( fp == NULL )
    {
        perror("popen error");
        return -1 ;
    }
    

    printf("音频正在播放....\n");

    // 打开用于发送指令的管道文件
    int fd_fifo = open(FIFO_PATH , O_RDWR );
    if (-1 == fd_fifo)
    {
        perror("open fifo error");
        return -1 ;
    }
    

    /*
        1.播放2.暂停/继续3.下一个视频4.结束5.快进5s
        6.退回5s
    */
    int i=1;//输入
    int n = 0;//播放/暂停
    int m = 0;//视频选择
    show_1152000bmp("music2.bmp", FB);
    while(1)//开始输入命令
    {
    project_touch();
 
    if(x<400)
    {
    
        if(y<240)
        {
            i=1;//播放
        }
        else
        {
            i=2;//暂停
        }

    }
    if(x>400)
    {
        if(y<240)
        {
            i=5;//快进
        }
        else
        {
            i=4;//退出
        }
    }


        switch (i)
        {
            case 1:
            fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
            break;
            case 2:
            send_cmd_music(fd_fifo,"pause\n");//暂停
            break;
            case 3:
            if(m == 0)
            {
                system("killall -9 /dev/mplayer");//结束播放器
                fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 1;
            }
            else
            {
                system("killall -9 /dev/mplayer");//结束播放器
                fp = popen("/dev/mplayer music.mp3 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 0;
            }
            break;
            case 4:
            send_cmd_music(fd_fifo,"quit\n");
            break;
            case 5:
            send_cmd_music(fd_fifo,"seek +5\n");
            case 6:
            send_cmd_music( fd_fifo , "seek -5\n" );
            break;
            case 0:
            continue;
            break;
            default:
            break;
        }
        

        sleep(1);
    

        pause();

     return 0;
    }

return 0;
}