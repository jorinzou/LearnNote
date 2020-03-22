#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>

void *task(void *arg)
{
	//分离的线程依然能被取消
	pthread_detach();
	printf("child tid=%d\n",syscall(SYS_gettid));
	while(1){
		sleep(2);	
	}
	//当这个线程被取消后，下面这句代码不会被执行
	printf("test");
}

int main(void)
{
	pthread_t t1;
	pthread_create(&t1,NULL,task,NULL);
	printf("parent tid=%d\n",syscall(SYS_gettid));
	while(1){
		int cmd = -1;
		scanf("%d",&cmd);
		pthread_cancel(t1);//取消t1线程，t1线程直接退出
	}
	return 0;
}
