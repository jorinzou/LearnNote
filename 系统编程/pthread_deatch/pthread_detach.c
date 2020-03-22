#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>

void *task(void *arg)
{
	//分离的线程，退出后，系统会自动回收资源,而不用父线程join
	pthread_detach(pthread_self());
	printf("child tid=%d\n",syscall(SYS_gettid));
	while(1){
		sleep(1);	
		exit(1);
	}
	return NULL;
}

int main(void)
{
	pthread_t t1;
	pthread_create(&t1,NULL,task,NULL);
	printf("parent tid=%d\n",syscall(SYS_gettid));
	while(1){
		sleep(1);
	}
	return 0;
}
