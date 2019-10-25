/*
自旋锁应用，及绑定cpu核实验
pthread_setaffinity_np
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

pthread_spinlock_t spinlock;


/*对线程进行绑核处理*/
int Setaffinity(int cpu_num, pthread_t t)
{
	cpu_set_t cpu_info = {0};

	__CPU_ZERO_S(sizeof (cpu_set_t), &cpu_info);
	__CPU_SET_S(cpu_num, sizeof (cpu_set_t), &cpu_info);

	if (0 != pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpu_info))	{
		DEBUG_INFO("set affinity failed");
		return 0;
	}
	return 1;
}

/*初始化互斥锁*/
void InitSpinLock(void)
{
	pthread_spin_init(&spinlock,PTHREAD_PROCESS_PRIVATE);           
}

/*销毁互斥锁*/
void DestorySpinLock(void)
{
	pthread_spin_destroy(&spinlock);
}

void* Thread1Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 1 wait get lock");
	pthread_spin_lock(&spinlock);
	DEBUG_INFO("thread 1 get lock");
	getchar();
	pthread_spin_unlock(&spinlock);
	DEBUG_INFO("thread 1  get lock end");
	printf("\n");
}

void* Thread2Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 2 wait get lock");
	pthread_spin_lock(&spinlock);
	DEBUG_INFO("thread 2 get lock");
	pthread_spin_unlock(&spinlock);
	DEBUG_INFO("thread 2  get lock end");
	printf("\n");
}

void* Thread3Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 3 wait get lock");
	pthread_spin_lock(&spinlock);
	DEBUG_INFO("thread 3 get lock");
	pthread_spin_unlock(&spinlock);
	DEBUG_INFO("thread 3 get lock end");
	printf("\n");
}

void* Thread4Task(void *arg)
{
	printf("\n");
	DEBUG_INFO("thread 4 wait get lock");
	pthread_spin_lock(&spinlock);
	DEBUG_INFO("thread 4 get lock");
	pthread_spin_unlock(&spinlock);
	DEBUG_INFO("thread 4 get lock end");
	printf("\n");
}

int main(void)
{
	InitSpinLock();
	
	pthread_t t1;
	pthread_create(&t1,NULL,Thread1Task,NULL);
	Setaffinity(0,t1);
	pthread_setname_np(t1, "thread_1");
	
	//保证线程2第二运行
	sleep(2);
	pthread_t t2;
	pthread_create(&t2,NULL,Thread2Task,NULL);
	Setaffinity(1,t2);
	pthread_setname_np(t2, "thread_2");
	
	//保证线程3第三运行
	sleep(2);
	pthread_t t3;
	pthread_create(&t3,NULL,Thread3Task,NULL);
	Setaffinity(2,t3);
	pthread_setname_np(t3, "thread_3");
	
	//保证线程4第四运行
	sleep(2);
	pthread_t t4;
	pthread_create(&t4,NULL,Thread4Task,NULL);
	Setaffinity(3,t4);
	pthread_setname_np(t4, "thread_4");
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	DEBUG_INFO("main thread exit");
	DestorySpinLock();
	return 0;
}