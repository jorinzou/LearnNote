#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>

void *task(void *arg)
{
	//������߳���Ȼ�ܱ�ȡ��
	pthread_detach();
	printf("child tid=%d\n",syscall(SYS_gettid));
	while(1){
		sleep(2);	
	}
	//������̱߳�ȡ�������������벻�ᱻִ��
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
		pthread_cancel(t1);//ȡ��t1�̣߳�t1�߳�ֱ���˳�
	}
	return 0;
}
