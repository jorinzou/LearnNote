/*
验证问题：多进程能不能使用互斥锁
结论:互斥锁对进程来说，是没有用的，因为每个进程间，是独立的地址空间,进程被创建之后，各个进程都有一份相同属性的mutex变量
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

pthread_mutex_t mutex;

/*初始化互斥锁*/
void InitMutex(void)
{
	pthread_mutex_init(&mutex,NULL);           
}

/*销毁互斥锁*/
void DestoryMutex(void)
{
	pthread_mutex_destroy(&mutex);

}

void ProcessTask_1(void)
{
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("process 1,pid = %d",getpid());
	sleep(6);
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("process 1 end life");
}

void ProcessTask_2(void)
{
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("process 2,pid = %d",getpid());
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("process 2 end life");
}

int main(void)
{
	InitMutex();
	
	pid_t pid = fork();
	if(pid == 0) {
		ProcessTask_1();
	}
	else {
		sleep(3);
		pid = fork();
		if (pid == 0) {
		  ProcessTask_2(); 
		}
	}
	
	while(1)
	{
		sleep(1);
	}
	DestoryMutex();
	return 0;
}