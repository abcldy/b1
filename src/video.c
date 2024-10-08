#include "main.h"

#define FIFO_PATH "/tmp/myFifo"


// FILE * fp;
// typedef enum//触摸屏方向结构体
//     {
//         SKIP_UNDEFINED  = -1,
//         SKIP_LEFT       = 0,
//         SKIP_RIGHT      = 1,
//         SKIP_UP         = 2,
//         SKIP_DOWN       = 3,
//     }TOUCH_DIRECTION;

// typedef struct {
//     struct input_event touchEvent;
//     int touch;
//     int x;
//     int y;
//     TOUCH_DIRECTION direction;
//                 }TOUCH;

// TOUCH touchInit()
    // {
    // TOUCH touch;
    // touch.touch = openFile("/dev/input/event0");
    // return touch;
    // }

//视频链表定义
// struct list_node{
// 	char* video;  //视频名字
	
// 	struct list_node *prev;
// 	struct list_node *next;
// };

// struct list_node n1,n2;
// n1.video = "Redial.mp4";n1.prev = &n2;n1.next = &n2;
// n2.video = "sb.mp4";n2.prev = &n1;n2.next = &n1;

// struct list_node *init_list_node()
// {
// 	struct list_node *head = NULL;
// 	head = (struct list_node *)malloc(sizeof(struct list_node));
// 	if(head == NULL)
// 	printf("malloc head error!\n");	
// 	head->prev = head;
// 	head->next = head;
	
// 	return head;
	
// };

int*plcd;
/*
    两个标志位(音量调节用)
*/
int exitFlag = 0;
int startstopFlag = 0;
int volume = 0;

int send_cmd(  int fd_fifo , const char * cmd )
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

// void drawRect(int x,int y,int width,int hight,int color)
// {
//     for (int i=x; i < x+width ; i++)
//     {
//         for (int j=y; j<y+hight ; i++)
//         {
//             *(plcd + j*800+i) = color;
//         }
//     }
// }

// void *changeVolume(void *arg)//修改音量
// {
//     char tempPath[100];
//     // TOUCH touchVolume = touchInit();
//     pthread_detach(pthread_self());
    
//     while(1)
//     {
//         drawRect(647,440,101,10,0x00D3D3D3);//x,y,w,h
//         drawRect(647,440,volume,10,0x000000FF);
//         if(volume >= 100)
//         {
//             volume = 100;
//         }
//         if(volume <= 0)
//         {
//             volume =0;
//         }
//         // getTouch(&touchVolume);

//         project_touch();
//         if (y>390 && y<470 && x >640 && x< 770)
//         {
//             volume = x-647;
//             sprintf(tempPath,"volume %d 1\n", volume);
//             int fd_fifo = open(FIFO_PATH , O_RDWR );
//             if(startstopFlag)
//             {
//                 write(fd_fifo,tempPath, strlen(tempPath));
//             }
//             if(exitFlag)
//             {
//                 break;
//             }
//         }
//     }
// }


int video(int argc, char const *argv[])
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
    

	// sprintf(buf,"/dev/mplayer -quiet -slave -zoom -x 600 -y 480 -input file=/tmp/myFifo & '/run/%s'",p->video);//拼接字符串到buf里
	// fp=popen(buf,"r"); 


    //直接使用 popen 来播放音视频
    FILE * fp = popen("/dev/mplayer /run/your_name.mp4 -quiet -slave -geometry 0:0 -zoom -x 600 -y 480 -input file=/tmp/myFifo &", "r");
    // startstopFlag = 1;
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

    show_location_bmp("bofangqi.bmp",600,0,FB);
    // pthread_t mythread;//线程
    // pthread_create(&mythread,NULL,changeVolume,NULL);


    while(1)//开始输入命令
    {   
        bool mm;//视频选择
        int n = 0;//播放/暂停
        
        // printf("1.播放 2.暂停/继续 3.下一个视频 4.退出 5.快进5s 6.退回5s \n");
        //触摸屏函数
        project_touch();
        //压力值判断下
        if (touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {

            if(x > 695 && x < 712 && y > 123 && y < 136)
            {
                send_cmd(fd_fifo,"quit\n");
                mm = !mm;
                printf("%d\n",&mm);
                if(mm)
                {
                    
                     fp = popen("/dev/mplayer /run/your_name.mp4 -quiet -slave -geometry 0:0 -zoom -x 600 -y 480 -input file=/tmp/myFifo &", "r");//上
                
                }
                else if(!mm)
                {
                    
                     fp = popen("/dev/mplayer /run/gui_mie.mp4 -quiet -slave -geometry 0:0 -zoom -x 600 -y 480 -input file=/tmp/myFifo &", "r");//上

                }
               
            }
            if(x > 643 && x < 662 && y > 201 && y < 220)
            {
                send_cmd(fd_fifo,"seek -5\n");//后退5秒
            }
            if(x > 690 && x < 720 && y > 270 && y < 300)
            {
               send_cmd(fd_fifo,"quit\n");
            //    m=~m;
               printf("%d\n",&mm);
               if(mm==0)
                { 
                    
                     fp = popen("/dev/mplayer /run/your_name.mp4 -quiet -slave -geometry 0:0 -zoom -x 600 -y 480 -input file=/tmp/myFifo &", "r");//上         
                
                }
                else if(mm==1)
                {
                    
                     fp = popen("/dev/mplayer /run/gui_mie.mp4 -quiet -slave -geometry 0:0 -zoom -x 600 -y 480 -input file=/tmp/myFifo &", "r");//上
                                    
                }//下
            
            }
            if(x > 745 && x < 764 && y > 201 && y < 220)
            {
                send_cmd(fd_fifo,"seek +5\n");//前进5秒
            }
            if(x > 688 && x < 719 && y > 169 && y < 226)
            {
                send_cmd(fd_fifo,"pause\n");//暂停
                // startstopFlag = ~startstopFlag;
            }

            if(x > 644 && x < 678 && y > 380 && y < 410)
            {
                send_cmd(fd_fifo,"volume -50\n");//音量-10
            }
            if(x > 735 && x <772 && y > 380 && y < 410)
            {
                send_cmd(fd_fifo,"volume +50\n");//音量+10
            }

            if(x > 770 && x < 800 && y > 0 && y < 30)
            {
                // exitFlag=1;
                system("killall -9 mplayer");
                interface();
                break;
            }
        }
    }

        // scanf("%d",&i);
        // switch (i)
        // {
        // case 1:
        //     fp = popen("/dev/mplayer  /home/gec/桌面/1234.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
        //     break;
        // case 2:
        //     send_cmd( fd_fifo , "pause\n" );//暂停
        //     break;
        // case 3:
        //     if(m == 0)
        //     {
        //         system("killall -9 /dev/mplayer");//结束播放器
        //         fp = popen("/dev/mplayer  /home/gec/桌面/1123.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
        //         m = 1;
        //     }
        //     else
        //     {
        //         system("killall -9 /dev/mplayer");//结束播放器
        //         fp = popen("/dev/mplayer  /home/gec/桌面/1234.mp4 -quiet -slave -input file=/tmp/myFifo &", "r");
        //         m = 0;
        //     }
        //     break;
        // case 4:
        //     send_cmd( fd_fifo , "quit\n" );
        //     break;
        // case 5:
        //     send_cmd( fd_fifo , "seek +5\n" );
        // case 6:
        //     send_cmd( fd_fifo , "seek -5\n" );
        //     break;
        // default:
        //     break;
        // }
        

       
    
    
    pause();

    return 0;
}