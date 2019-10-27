/*
线程屏障
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

pthread_barrier_t barrier;
int g_value = 0;

/*初始化屏障*/
void InitBarrier(void)
{
	pthread_barrier_init(&barrier,NULL,4);
}

/*销毁屏障*/
void DestoryBarrier(void)
{
	pthread_barrier_destroy(&barrier);
}

void *task1(void *arg)
{
	while(1)
	{
		DEBUG_INFO("%ld",pthread_self());
		pthread_barrier_wait(&barrier);
		DEBUG_INFO("%ld,%d",pthread_self(),g_value++);
		break;
	}
}

int main(void)
{
	InitBarrier();
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	pthread_t t4;

	pthread_create(&t1,NULL,task1,NULL);
	sleep(2);
	pthread_create(&t2,NULL,task1,NULL);
	sleep(2);
	pthread_create(&t3,NULL,task1,NULL);
	sleep(2);
	pthread_create(&t4,NULL,task1,NULL);

	pthread_join(t1,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	pthread_join(t2,NULL);
	DestoryBarrier();
	return 0;
}
