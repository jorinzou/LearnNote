#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>

static int set_affinity(int cpu_num, pthread_attr_t* attr)
{
	cpu_set_t cpu_info = {0};

	__CPU_ZERO_S(sizeof (cpu_set_t), &cpu_info);
	__CPU_SET_S(cpu_num, sizeof (cpu_set_t), &cpu_info);

	if (0 != pthread_attr_setaffinity_np(attr, sizeof(cpu_set_t), &cpu_info)){
		printf("set affinity failed");
		return 0;
	}

	return 1;
}

void *task1(void *arg)
{
	while(1){
		sleep(1);
	}
}

void *task2(void *arg)
{
	while(1){
		sleep(1);
	}
}

void *task3(void *arg)
{
	while(1){
		sleep(1);
	}
}

int main(void)
{
	pthread_attr_t t1_attr = {0};
	pthread_attr_t t2_attr = {0};
	pthread_attr_t t3_attr = {0};
	
	set_affinity(1,&t1_attr);
	set_affinity(2,&t2_attr);
	set_affinity(3,&t3_attr);
	
	pthread_t t1;
	pthread_create(&t1,&t1_attr,task1,NULL);
	pthread_setname_np(t1, "t1");

	pthread_t t2;
	pthread_create(&t2,&t2_attr,task2,NULL);
	pthread_setname_np(t2, "t2");

	pthread_t t3;
	pthread_create(&t3,&t3_attr,task3,NULL);
	pthread_setname_np(t3, "t3");

	printf("parent tid=%d\n",syscall(SYS_gettid));
	pthread_setname_np(pthread_self(), "t4");
	while(1){
		sleep(1);
	}
	return 0;
}
