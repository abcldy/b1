#include"main.h"
#define NUM 3
char * arr_bmpname[NUM] = {"b2.bmp","111.bmp","gameover.bmp"};//图片名
int i = 0;//记录此刻播放的图片
int x5,y5;

void project_touch2()
{
    read(touch_fd,&touch,sizeof(touch));//printf("读取触摸屏文件完成\n")
    if (touch.type == EV_ABS && touch.code == ABS_X) x5 = touch.value*800/1024;//黑板
    if (touch.type == EV_ABS && touch.code == ABS_Y) y5 = touch.value*480/600;//黑板
    // if (touch.type == EV_ABS && touch.code == ABS_X) x = touch.value;//
    // if (touch.type == EV_ABS && touch.code == ABS_Y) y = touch.value;//
}

typedef struct photo_hub//图片名结构体
{
    int no;
    char bmp_name[30];
    struct photo_hub* next;
}ph;

ph* photo_list(char*name[NUM])//根据数组创建链表
{
    ph*s,*r;
    ph* head = (ph*)malloc(sizeof(ph));//创建头节点

    head->no = -1;
    r = head;//r指向尾节点，开始时指向头指针

    for(int i=0;i<NUM;i++)
    {
        s = (ph*)malloc(sizeof(ph));
        s->no = i;
        strcpy(s->bmp_name,arr_bmpname[i]);
        r->next = s;
        r = s;
    }
    //r->next = NULL;//最后一个元素next置空
    r->next = head;//构成循环链表
    return head;
}

void show_list(ph* head)//展示列表
{
    printf("list is :\n");
    ph *a = head;
    if(a == NULL)
    {
        printf("head is empty!");
    }
    else
    {
        printf("head : no = %d ,bmp_name = %s\n",a->no,a->bmp_name);
        a = a->next;
        while(a != head)
        {
            printf("no = %d ,bmp_name = %s\n",a->no,a->bmp_name);
            a = a->next;
        }
    }
}


void free_space(ph* head)//释放空间
{
    ph *p = head->next;
    ph* q = p->next;
    while(p != head)
    {
        printf("node no = %d is free\n",p->no);
        free(p);
        p = q;
        q = q->next;
    }
    printf("node no = %d is free\n",p->no);
    free(p);
    printf("list is free now!\n");
}

void show_album(int i,ph* head)//展示图片
{
    ph* p = head;
    if(head == NULL)
    {
        printf("list is empty,can't read !\n");
        return ;
    }
    while(1)
    {
        if(p->no != i)
        {
            p = p->next;
        }
        else
        {
            break;
        }
    }
    show_1152000bmp(p->bmp_name,FB);
    p = NULL;
}

int getslide()
{
    int flag = 0;//获得的坐标的数量
    int x1,y1,x2,y2;
    while(1)
    {
        read(touch_fd , &touch , sizeof(touch));
        if (touch.type == EV_ABS && touch.code == ABS_X && flag == 0) {x1 = touch.value*800/1024;flag = 1;}
        if (touch.type == EV_ABS && touch.code == ABS_Y && flag == 1) {y1 = touch.value*480/600;flag = 2;}
        if(flag == 2){flag = 0;break;}

    }
    x2 = x1;y2 = y1;
    while(1)
    {
        read(touch_fd , &touch , sizeof(touch));
        if (touch.type == EV_ABS && touch.code == ABS_X) {x2 = touch.value*800/1024;}
        if (touch.type == EV_ABS && touch.code == ABS_Y) {y2 = touch.value*480/600;}    
        if(touch.type == EV_KEY && touch.code == BTN_TOUCH && touch.value == 0)
        {
            //压力值为0的时候此时获取终点坐标
            break;	
        }	
    
    }

    if(x2 > x1 && x2*x2 - x1*x1 > y2*y2 - y1*y1){return 1;}//右划
    if(x2 < x1 && x1*x1 - x2*x2 > y1*y1 - y2*y2){return 2;}//左划
    if(y2 < y1 && y1*y1 - y2*y2 > x1*x1 - x2*x2){return 3;}//上划
    if(y2 > y1 && y2*y2 - y1*y1 > x2*x2 - x1*x1){return 4;}//下划
    if(x2 == x1 && y2 == y1){return 5;}//点击
}

void delete_node(int l,ph* head)
{
    ph* p = head; int num = -1;
    ph* q = p->next;
    while(1)
    {
        if(q->no != l)
        {
            p = q;
            q = q->next;
        }
        else
        {
            break;
        }
    }
    //q->no = -1;
    num = q->no;
    p->next = q->next;
    free(q);q = NULL;
    printf("the photo %d is deleted well!\n",num);
}

void photo_delete(ph* head)//删除图片
{
    show_location_bmp("dustbin.bmp",200,350,FB);
    project_touch2();
    if(x5 >= 200-30 && x5 <= 200+50+30 && y5 >= 350-30 && y5 <= 350+50+30)
    {
        delete_node(i,head);
        i++;
    }
}


int touchscreen()//相册功能
{

    // int j = 0;
    ph* pho = photo_list(arr_bmpname);//初始化链表
	while(1)
	{
        int list = getslide();//记录滑动的类型
        printf("list = %d\n",list);
        if(list == 5)
        {
            project_touch();



            if(x <= 400){i--;}
            if(x >= 400){i++;}
        }
        if(list == 1){i--;}
        if(list == 2){i++;}
        if(list == 3){break;}
        

        if(i == -1){i = NUM - 1;}
        if(i == NUM){i = 0;}

        
        // if(i == j){continue;}//如果没有动作则跳出本次循环
        // //show_1152000bmp(arr_bmpname[i],FB);

        show_list(pho);//查看列表
        show_album(i,pho);//展示图片

        //图片删除模块
        photo_delete(pho);

        printf("i = %d\n",i);   

        // j  = i;
    }
	printf("out of loop\n");
    free_space(pho);
    
}

int photo_album()//相册部分
{
    show_1152000bmp(arr_bmpname[0], FB);//显示第一张图片
    touchscreen();
    printf("out of album\n");
    show_1152000bmp("interface.bmp", FB);//回到主界面
    return 0;
}

//pthread