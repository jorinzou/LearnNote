#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

/*
线程A获得信号量成功后，B,C线程去获取信号量阻塞，当A释放信号量的时候，哪个线程先获得信号量
先获得调度的线程先获得信号量
*/

sem_t sem;

void SemInit(void)
{
	sem_init(&sem,0,1);
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
	SemInit();
	pthread_t thread;
	pthread_create(&thread,NULL,task1,NULL);

	sleep(2);
	pthread_t thread2;
	pthread_create(&thread2,NULL,task2,NULL);
	
	sleep(2);
	pthread_t thread3;
	pthread_create(&thread3,NULL,task3,NULL);
	
	sleep(2);
	pthread_join(thread,NULL);
	pthread_join(thread2,NULL);
	pthread_join(thread3,NULL);

	SemDestroy();
	return 0;
}