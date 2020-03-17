#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

struct conf_mgr
{
    int print_switch;
    int log_switch;
};

int shmid = -1;
struct conf_mgr *p = NULL;

void fa(int signum)
{
	//脱接共享内存
	int ret = shmdt(p);
	if (ret < 0){
		perror("shmdt");
		DEBUG_INFO("%s",strerror(ret));
		exit(-1);
	}
	DEBUG_INFO("脱接共享内存成功");
}

//初始化共享内存，并获取共享内存shmid
void init_shm(void)
{
	//1.获取key值，使用ftok函数
	key_t key = ftok(".",100);
	if(-1 == key)
	{
		perror("ftok"),exit(-1);
	}
	DEBUG_INFO("key = %#x",key);
	//2.获取共享内存，使用shmget函数
	shmid = shmget(key,0,0);
	if(-1 == shmid)
	{
		perror("shmget"),exit(-1);
	}
	DEBUG_INFO("shmid = %d",shmid);
}

//获取共享内存内存指针，并赋值
int main(void)
{
	init_shm();
	if(SIG_ERR == signal(SIGINT,fa)){
		perror("signal"),exit(-1);
	}
	while(1){
		void* pv = shmat(shmid,NULL,0);
		if((void*)-1 == pv)
		{
			perror("shmat"),exit(-1);
		}
		DEBUG_INFO("挂接共享内存成功\n");
		//4.访问共享内存
		p = (struct conf_mgr*)pv;
		int a = getchar();
		if (a != '\n')
		{
			p->print_switch = a;
			p->log_switch = a+1;
		}
		sleep(1);
	}
	return 0;
}