/*
条件变量
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

pthread_cond_t cond;
pthread_mutex_t mutex;
int g_value = 0;

/*对线程进行绑核处理*/
int Setaffinity(int cpu_num, pthread_t t)
{
	cpu_set_t cpu_info = {0};
	__CPU_ZERO_S(sizeof (cpu_set_t), &cpu_info);
	__CPU_SET_S(cpu_num, sizeof (cpu_set_t), &cpu_info);

	if (0 != pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpu_info)){
		DEBUG_INFO("set affinity failed");
		return 0;
	}
	return 1;
}

/*初始化条件变量*/
void InitCondLock(void)
{
	pthread_cond_init(&cond,NULL);
	pthread_mutex_init(&mutex,NULL);
}

/*销毁条件变量*/
void DestoryCondLock(void)
{
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void *task1(void *arg)
{
	int ret = -1;
	while(1){
		pthread_mutex_lock(&mutex);
		ret = pthread_cond_wait(&cond,&mutex);
		DEBUG_INFO("thread 1 g_value=%d,return result:%s",g_value,strerror(ret));
		pthread_mutex_unlock(&mutex);
	}
}

void *task2(void *arg)
{
	int ret = -1;
	while(1){
		int a = getc(stdin);//输入小写字母a
		if (a == 97){
			g_value++;
			//ret = pthread_cond_signal(&cond);
			ret = pthread_cond_broadcast(&cond);
			DEBUG_INFO("thread 2,return result:%s",strerror(ret));
		}
	}
}	

void *task3(void *arg)
{
	int ret = -1;
	while(1){
		pthread_mutex_lock(&mutex);
		ret = pthread_cond_wait(&cond,&mutex);
		DEBUG_INFO("thread 3 g_value=%d,return result:%s",g_value,strerror(ret));
		pthread_mutex_unlock(&mutex);
	}
}	

void *task4(void *arg)
{
	int ret = -1;
	struct timespec ab;
	
	while(1){
		pthread_mutex_lock(&mutex);
		ab.tv_sec = time(NULL) + 3;//3s后返回
		ab.tv_nsec = 0;
		ret = pthread_cond_timedwait(&cond,&mutex,&ab);
		DEBUG_INFO("thread 4 g_value=%d,return result:%s",g_value,strerror(ret));
		pthread_mutex_unlock(&mutex);
	}
}	

int main(void)
{
	InitCondLock();
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	pthread_t t4;

	pthread_create(&t1,NULL,task1,NULL);
	pthread_create(&t2,NULL,task2,NULL);
	sleep(2);
	pthread_create(&t3,NULL,task3,NULL);
	sleep(2);
	pthread_create(&t4,NULL,task4,NULL);

	pthread_join(t1,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	pthread_join(t2,NULL);
	DestoryCondLock();
	return 0;
}
