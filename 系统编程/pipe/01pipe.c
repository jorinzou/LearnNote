//无名管道
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int pipefd[2];
//创建无名管道
void CreatePipe(void)
{
	int res = pipe(pipefd);
	if(-1 == res){
		perror("pipe"),exit(-1);
	}
	DEBUG_INFO("创建无名管道成功");
}


//写管道线程
void *process(void *arg)
{
	char buf[4096];
	int ret;
	int size = 0;
	pthread_detach(pthread_self());
	while(1){
		memset(buf,1,sizeof(buf));
		sleep(1);
		ret = write(pipefd[1],buf,sizeof(buf));
		if(ret<0){
			perror("write");
		}
		size+=ret;
		DEBUG_INFO("child ret=%d,size=%d",ret,size);
	}
}

int main(void)
{
	CreatePipe();
	pthread_t t;
	pthread_create(&t,NULL,process,&t);
	
	char buf[4096];
	int ret;
	int i = 1;
	
	//主线程读管道
	while(1){
		getchar();
		ret = read(pipefd[0],buf,sizeof(buf));
		if(ret < 0){
			perror("read");
			exit(-1);
		}
		DEBUG_INFO("parent ret=%d,i=%d",ret,i++);
	}
	return 0;
}
