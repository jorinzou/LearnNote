/*
三个以上线程，当一个线程获得互斥锁，其它线程等待，当获得的锁的线程解除锁定，等待的线程哪个先获得锁的使用权
结论:从以下的实现，可以得出，先运行的线程，获得锁的线程释放锁后，先运行的线程先获得锁的使用权
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

void* Thread1Task(void *arg)
{
	printf("\n\n\n");
	DEBUG_INFO("thread 1 wait get lock");
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("thread 1 get lock");
	getchar();
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("thread 1  get lock end");
	printf("\n\n\n");
}

void* Thread2Task(void *arg)
{
	printf("\n\n\n");
	DEBUG_INFO("thread 2 wait get lock");
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("thread 2 get lock");
	getchar();
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("thread 2  get lock end");
	printf("\n\n\n");
}

void* Thread3Task(void *arg)
{
	printf("\n\n\n");
	DEBUG_INFO("thread 3 wait get lock");
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("thread 3 get lock");
	getchar();
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("thread 3  get lock end");
	printf("\n\n\n");
}

void* Thread4Task(void *arg)
{
	printf("\n\n\n");
	DEBUG_INFO("thread 4 wait get lock");
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("thread 4 get lock");
	getchar();
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("thread 4  get lock end");
	printf("\n\n\n");
}

int main(void)
{
	InitMutex();
	
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
	DestoryMutex();
	return 0;
}