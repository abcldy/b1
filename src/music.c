#include<main.h>

void music1()
{
while(1)
    {
        int i;
       scanf("%d",&i);
        if(i==1)
        {
        /*播放音乐*/
     system("/usr/bin/madplay music.mp3 &");
    
         }  
    if(i==2)
        {
        /*暂停音乐*/
     system("killall -19 /usr/bin/madplay");

        }
   if(i==3)
   {
         /*继续音乐*/
     system("killall -18 /usr/bin/madplay");

   }
   if(i==4)
   {
        /*结束音乐*/
     system("killall -9 /usr/bin/madplay"); 
   }
    
    }
    

    
}


// #include <stdio.h>
// #include <stdlib.h>

// int main()
// {

//     while(1)
//     {
//         int i;
//        scanf("%d",&i);
//         if(i==1)
//         {
//         /*播放音乐*/
//      system("/usr/bin/madplay music.mp3 &");
    
//          }  
//         if(i==2)
//         {
//         /*暂停音乐*/
//      system("killall -SIGSTOP /usr/bin/madplay");

//         }
//    if(i==3)
//    {
//          /*继续音乐*/
//      system("killall -18 /usr/bin/madplay");

//    }
//    if(i==4)
//    {
//         /*结束音乐*/
//      system("killall -9 /usr/bin/madplay"); 
//    }
    
//     }
    

//     return 0;
// }