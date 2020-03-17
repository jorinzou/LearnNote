#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

/*
线程A获得信号量成功后，B线程sem_waitdwait获取信号量,阻塞一定的时间再返回，注意第二个参数是时间戳
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
	struct timespec ts;
	ts.tv_sec = time(NULL) + 6;
	ts.tv_nsec = 1;
	
	printf("\n");
	DEBUG_INFO("thread 2 time wait get sem");
	int ret = -1;
	while(ret)
	{
		time_t t1;
		time_t t2;
		t1 = time(NULL);
		ret = sem_timedwait(&sem,&ts);
		t2 = time(NULL);
		DEBUG_INFO("%d",t2-t1);
		//DEBUG_INFO("ret=%d",ret);
		if (errno == EINTR){
			DEBUG_INFO("EINTR:%d",EINTR);
		}
		if(errno == EAGAIN){
			DEBUG_INFO("EAGAIN:%d",EAGAIN);
		}
		
		if(errno == EINVAL){
			DEBUG_INFO("EINVAL:%d",EINVAL);
		}
		
		if(errno == ETIMEDOUT){
			DEBUG_INFO("ETIMEDOUT:%d",ETIMEDOUT);
		}
		sleep(1);
	}
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
	DEBUG_INFO("main thread end");
	return 0;
}