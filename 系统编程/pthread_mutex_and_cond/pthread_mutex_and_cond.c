#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int max = 5;
int cur = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *task2(void *arg);

void *task1(void *arg)
{
	pthread_detach(pthread_self());
	while(1){
		pthread_mutex_lock(&mutex);
		while (max == cur){
			printf("cond wait before\n");
			pthread_t t2;
			pthread_create(&t2,NULL,task2,NULL);
			pthread_cond_wait(&cond,&mutex);
			printf("cond wait after\n");
		}
		sleep(1);
		printf("task1:cur=%d,max=%d\n",cur,max);
		cur++;
		pthread_mutex_unlock(&mutex);
	}
}

void *task2(void *arg)
{
	pthread_detach(pthread_self());
	int res = 0;
	pthread_mutex_lock(&mutex);
	if(--cur == (res = max -1)){
		printf("test\n");
		pthread_cond_signal(&cond);
	}
	sleep(1);
	printf("task2:cur=%d,res=%d\n",cur,res);
	pthread_mutex_unlock(&mutex);
	printf("task2 unlock mutex\n");
}

int main(void)
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_t t1;
	pthread_create(&t1,NULL,task1,NULL);
#if 0
	sleep(1);
	pthread_t t2;
	pthread_create(&t2,NULL,task2,NULL);
#endif

	while(1){
		sleep(1);
	}
	return 0;
}
