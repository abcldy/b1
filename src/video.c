#include"main.h"

#define FIFO_PATH "/tmp/myFifo"


int send_cmd(int fd_fifo,const char * cmd)
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


int video_show(int argc, char const *argv[])
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
    FILE * fp = popen("mplayer  /home/gec/桌面/1123.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");

    /*
        mplayer  3.avi  -geometry 100:200 -zoom -x 400 -y 200 &
        -geometry 100:200   设置视频播放的位置 
        -zoom -x 400 -y 200  设置视频的宽度和高度
    */

    if ( fp == NULL )
    {
        perror("popen error");
        return -1 ;
    }
    
    printf("视频正在播放....\n");

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
    int i=0;//输入
    int n = 0;//播放/暂停
    int m = 0;//视频选择
    while(1)//开始输入命令
    {
        printf("1.播放 2.暂停/继续 3.下一个视频 4.退出 5.快进5s 6.退回5s \n");
        scanf("%d",&i);
        switch (i)
        {
        case 1:
            fp = popen("mplayer  /home/gec/桌面/1234.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
            break;
        case 2:
            send_cmd( fd_fifo , "pause\n" );//暂停
            break;
        case 3:
            if(m == 0)
            {
                system("killall -9 mplayer");//结束播放器
                fp = popen("mplayer  /home/gec/桌面/1123.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 1;
            }
            else
            {
                system("killall -9 mplayer");//结束播放器
                fp = popen("mplayer  /home/gec/桌面/1234.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
                m = 0;
            }
            break;
        case 4:
            send_cmd( fd_fifo , "quit\n" );
            break;
        case 5:
            send_cmd( fd_fifo , "seek +5\n" );
        case 6:
            send_cmd( fd_fifo , "seek -5\n" );
            break;
        default:
            break;
        }
        

        sleep(3);
    }

    pause();

    return 0;
}


int project_video()
{
    return 0;
}