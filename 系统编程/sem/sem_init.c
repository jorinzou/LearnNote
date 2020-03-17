#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

/*
当sem_init第三个参数大于1时会怎样
其它线程不会被阻塞，可以锁住信号量
sem_destroy没有清空sem中的初始值
*/

sem_t sem;

void SemInit(void)
{
	sem_init(&sem,0,2);
}

void SemDestroy(void)
{
	sem_destroy(&sem);
}

void *task1(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 1 wait get sem");
	sem_wait(&sem);
	DEBUG_INFO("thread 1 user sem");
	getchar();
	sem_post(&sem);
	DEBUG_INFO("thread 1 free sem");
	printf("\n");
}

void *task2(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 2 wait get sem");
	sem_wait(&sem);
	DEBUG_INFO("thread 2 user sem");
	sem_post(&sem);
	DEBUG_INFO("thread 2 free sem");
	printf("\n");
}

void *task3(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 3 wait get sem");
	sem_wait(&sem);
	DEBUG_INFO("thread 3 user sem");
	sem_post(&sem);
	DEBUG_INFO("thread 3 free sem");
	printf("\n");
}

int main(void)
{
	int p = -1;
	SemInit();
	sem_getvalue(&sem,&p);
	DEBUG_INFO("1p=%d",p);
	pthread_t thread;
	pthread_create(&thread,NULL,task1,NULL);
	sem_getvalue(&sem,&p);
	DEBUG_INFO("2p=%d",p);

	sleep(2);
	pthread_t thread2;
	pthread_create(&thread2,NULL,task2,NULL);
	sem_getvalue(&sem,&p);
	DEBUG_INFO("3p=%d",p);
	
	sleep(2);
	pthread_t thread3;
	pthread_create(&thread3,NULL,task3,NULL);
	sem_getvalue(&sem,&p);
	DEBUG_INFO("5p=%d",p);
	
	sleep(2);
	pthread_join(thread,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);
	sem_getvalue(&sem,&p);
	DEBUG_INFO("6p=%d",p);

	SemDestroy();
	sem_getvalue(&sem,&p);
	DEBUG_INFO("7p=%d",p);
	return 0;
}