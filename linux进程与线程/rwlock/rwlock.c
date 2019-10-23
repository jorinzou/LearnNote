#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
1线程获得写锁
2,3,4线程，先获得内核调试的线程，先执行,并获得锁
*/

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

pthread_rwlock_t RwLock;

void InitRwLock(void)
{
	pthread_rwlock_init(&RwLock,NULL);           
}

void DestoryRwLock(void)
{
	pthread_rwlock_destroy(&RwLock);
}

void* Thread1Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 1 wait get lock");
	pthread_rwlock_wrlock(&RwLock);
	DEBUG_INFO("thread 1 get lock");
	getchar();
	pthread_rwlock_unlock(&RwLock);
	DEBUG_INFO("thread 1  get lock end");
	printf("\n");
}

void* Thread2Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 2 wait get lock");
	pthread_rwlock_rdlock(&RwLock);
	DEBUG_INFO("thread 2 get lock");
	pthread_rwlock_unlock(&RwLock);
	DEBUG_INFO("thread 2  get lock end");
	printf("\n");
}

void* Thread3Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 3 wait get lock");
	pthread_rwlock_rdlock(&RwLock);
	DEBUG_INFO("thread 3 get lock");
	pthread_rwlock_unlock(&RwLock);
	DEBUG_INFO("thread 3  get lock end");
	printf("\n");
}

void* Thread4Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 4 wait get lock");
	pthread_rwlock_rdlock(&RwLock);
	DEBUG_INFO("thread 4 get lock");
	pthread_rwlock_unlock(&RwLock);
	DEBUG_INFO("thread 4  get lock end");
	printf("\n");
}

int main(void)
{
	InitRwLock();
	
	pthread_t t1;
	pthread_create(&t1,NULL,Thread1Task,NULL);
	
	//保证线程2第二运行
	sleep(2);
	pthread_t t2;
	pthread_create(&t2,NULL,Thread2Task,NULL);
	
	//保证线程3第三运行
	sleep(2);
	pthread_t t3;
	pthread_create(&t3,NULL,Thread3Task,NULL);
	
	//保证线程4第四运行
	sleep(2);
	pthread_t t4;
	pthread_create(&t4,NULL,Thread4Task,NULL);
	
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	DEBUG_INFO("main thread exit");
	DestoryRwLock();
	return 0;
}