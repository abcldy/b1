#include "main.h"

int main()
{
    

    /*项目初始化*/
    project_init();

    /*主界面*/
    jinru();

    /*项目结束释放*/
    project_free();

    return 0;
}

/*
    arm-linux-gcc ./src/*.c -I ./inc -lpthread -o ./bin/b1
*/