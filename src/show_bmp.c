#include "main.h"


int show_1152000bmp(char *pathname, int *show_1152000bmp_fd)
{
	//1、打开BMP格式图片和lcd显示屏
	int fd_bmp = open(pathname,  O_RDWR);
	if(fd_bmp == -1)
	{
		printf("show_1152000bmp(), open bmp fail!\n");
		return -1;
	}
	
	//2、去掉54个字节
	int ret = lseek(fd_bmp, 54, SEEK_SET);
	if(ret == -1)
	{
		printf("show_1152000bmp(), lseek bmp fail!\n");
		return -1;		
	}
	
	//3、读取BMP数据到lcd屏幕中
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	bzero(bmp_buf, 800*480*3);
	bzero(lcd_buf, 800*480);
	
	ret = read(fd_bmp, bmp_buf, 800*480*3);//BMP数据放到了bmp_buf中
	if(ret == -1)
	{
		printf("show_1152000bmp(), read bmp fail!\n");
		return -1;		
	}		

	//4、关闭文件，回收资源
	close(fd_bmp);
	
	//5、800*480个A像素变成800*480个B像素算法。
	int i;
	for(i=0; i<800*480; i++)
	{
		lcd_buf[i] = bmp_buf[i*3]<<0 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16;
		//lcd_buf[0] = bmp_buf[0]<<0 | bmp_buf[1]<<8 | bmp_buf[2]<<16;
		//lcd_buf[1] = bmp_buf[3]<<0 | bmp_buf[4]<<8 | bmp_buf[5]<<16;
		//lcd_buf[2] = bmp_buf[6]<<0 | bmp_buf[7]<<8 | bmp_buf[8]<<16;
	}
	
	//6、lcd_buf数据（DDR2中的数据）通过mmap显示到LCD屏幕中。
/*
	for(i=0; i<800*480; i++)
	{
		*(lcd_mmap+i)=lcd_buf[i];
	}	
*/
	int x, y;
	for(y=0; y<480; y++)//优化算法，解决图片颠倒问题
	{
		for(x=0; x<800; x++)
		{
			*(show_1152000bmp_fd+800*(479-y)+x)= lcd_buf[800*y+x];
		}
	}
			
	return 0;
}

int show_location_bmp(char *pathname,int lcd_x_coordinates, int lcd_y_coordinates, int wide, int height,int *show_bmp_lcd_mmap) 
{
	int i;
	int ret;
	int x, y;
	
	char bmp_buf[wide*height*3];
	int  new_buf[wide*height];
	bzero(bmp_buf, sizeof(bmp_buf));
	bzero(new_buf, sizeof(new_buf));

	int bmp_fd = open(pathname, O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("show_location_bmp(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd,54,SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		printf("show_location_bmp(), lseek bmp failed\n");		
		return -1;
	}

    int *new_p = show_bmp_lcd_mmap + 800*lcd_y_coordinates + lcd_x_coordinates;//3、重新确定映射位置。
	
	ret = read(bmp_fd, bmp_buf, wide*height*3);//4、取读图片像素
	if(ret == -1)
	{
		printf("show_location_bmp(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片
	
	for(i=0; i<wide*height; i++)//6、24bits 转 32bits控制变量
	{
			new_buf[i] = bmp_buf[i*3]<<0  |  bmp_buf[(i*3)+1]<<8 | bmp_buf[(i*3)+2]<<16;		
	}
	
	for(y=0;y<height;y++)//7、解决图片上下颠倒问题
	{
		for(x=0;x<wide;x++)
		{
			*(new_p+(800*((height-1)-y))+x) = new_buf[wide*y+x];
		}
	}
	
	return 0;		
}