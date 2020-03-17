#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/syscall.h>
#include <pthread.h>

void fun(int signum)
{
	printf("---------------------------------------\n");
	printf("signum=%d\n",signum);
	printf("sig num pid=%d\n",syscall(SYS_gettid));
}

void* thread_1(void *arg)
{
	pthread_detach(pthread_self());

	printf("t1 pid=%d\n",syscall(SYS_gettid));
	while(1)
	{
		long long  i = 0;
		i++;
		if (i > 10000000000){
			i = 0;
		}
	}
}

int main(void)
{
	struct sigaction action;
	action.sa_handler = fun;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT,&action,NULL);

	pthread_t t1;
	pthread_create(&t1,NULL,thread_1,NULL);

	printf("main pid=%d\n",syscall(SYS_gettid));
	while(1)
	{
		sleep(10);
	}
	return 0;
}
