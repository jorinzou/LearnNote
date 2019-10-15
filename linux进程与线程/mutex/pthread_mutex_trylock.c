/*
验证:pthread_mutex_trylock不论成功与否，直接返回，成功时返回0
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
	pthread_mutex_lock(&mutex);
	DEBUG_INFO("get metux 1");
	getchar();
	pthread_mutex_unlock(&mutex);
	DEBUG_INFO("thread 1 unlock");
}

void* Thread2Task(void *arg)
{
	int ret = -1;
	while(1)
	{
		ret = pthread_mutex_trylock(&mutex);
		if (ret == 0)
		{
			goto END;
		}
		DEBUG_INFO("trylock agin ,ret=%d",ret);
		sleep(1);
	}

END:
	pthread_mutex_unlock(&mutex);
}

int main(void)
{
	InitMutex();
	
	pthread_t t1;
	pthread_create(&t1,NULL,Thread1Task,NULL);
	
	
	sleep(2);//保证线程1先运行，先获得互斥锁
	pthread_t t2;
	pthread_create(&t2,NULL,Thread2Task,NULL);
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	DEBUG_INFO("main thread exit");
	DestoryMutex();
	return 0;
}