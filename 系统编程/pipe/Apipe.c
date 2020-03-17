//有名管道

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

//创建一个不存在的管道文件
void create_fifo_file(void)
{
	DIR *dir;
    struct dirent *ptr;
	//打开目录 
    if ((dir=opendir("./")) == NULL){
        perror("opendir");
       	exit(-1);
    }
	
	//遍历目录，查看当前是否存在相应的管道文件,如果不存在，则创建
	while ((ptr=readdir(dir)) != NULL) {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0) 
            continue;
        if(strcmp(ptr->d_name,"a.pipe") == 0)
        {
            DEBUG_INFO("file:%s",ptr->d_name);
			closedir(dir);
			return;
        }
    }
	
	//创建管道文件
	int ret = mkfifo("./a.pipe",0664);
	if (ret < 0){
		perror("mkfifo");
		exit(-1);
	}
    closedir(dir);
}


#define BUF " pipe write "
int main(void)
{
	char buf[4096] = {0};
	memset(buf,1,sizeof(buf));
	create_fifo_file();
	int fd = open("./a.pipe",O_RDWR);
	if (fd < 0){
		perror("open");
		exit(-1);
	}
	
	int i = 1;
	while(1){
		sleep(1);
		//往管道写东西
		int ret = write(fd,buf,sizeof(buf));
		if (ret < 0){
			perror("write");
			return -1;
		}
		//当i等于16时，写入会被阻塞，大概可以证明，当前内核的pipe缓冲区大小为16x4096
		DEBUG_INFO("ret=%d,i=%d",ret,i++);
	}
	return 0;
}