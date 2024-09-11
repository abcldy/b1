

#include <stdio.h>
#include <stdlib.h>

int main()
{

    while(1)
    {
        int i;
       scanf("%d",&i);
        if(i==1)
        {
        /*播放音乐*/
     system("mplayer music.mp3 &");
    
         }  
        if(i==2)
        {
        /*暂停音乐*/
     system("killall -SIGSTOP mplayer");

        }
   if(i==3)
   {
         /*继续音乐*/
     system("killall -18 mplayer");

   }
   if(i==4)
   {
        /*结束音乐*/
     system("killall -9 mplayer"); 
   }
    
    }
    

    return 0;
}