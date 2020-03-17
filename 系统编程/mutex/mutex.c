/*
验证问题：同一个线程多次获取锁，会造成死锁
结论：同一个线程多次使用互斥锁，会造成死锁
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

/*同一个线程多次获取锁，造成死锁*/
void* Thread1Task(void *arg)
{
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("get metux 1");
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("get metux 2");
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex);
}

int main(void)
{
	InitMutex();
	
	pthread_t t1;
	pthread_create(&t1,NULL,Thread1Task,NULL);
	
	pthread_join(t1,NULL);
	DEBUG_INFO("main thread exit");
	DestoryMutex();
	return 0;
}