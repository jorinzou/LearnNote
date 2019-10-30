#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
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
	
	//调用shmctl函数实现删除
	ret = shmctl(shmid,IPC_RMID/*这个参数表示删除共享内存的意思*/,NULL);
	if(-1 == ret)
	{
		perror("shmctl"),exit(-1);
	}
	printf("删除共享内存成功\n");
	exit(0); //终止进程
}

//创建共享内存
int init_shm(void)
{
    key_t key = ftok(".",100);
	if (-1 == key)
	{
		perror("ftok");
        return -1;
	}
	DEBUG_INFO("key = %#x",key);

	//创建共享内存
    shmid = shmget(key,sizeof(struct conf_mgr),IPC_CREAT|0664);
	if (-1 == shmid)
	{
		perror("shmget");
        return -1;
	}
	DEBUG_INFO("shmid = %d",shmid);
    return 0;
}

//获取共享内存
struct conf_mgr *get_shm(int shmid)
{
    void* pv = shmat(shmid,NULL,0);
	if ((void*)-1 == pv)
	{
		perror("shmat");
        return NULL;
	}
	//DEBUG_INFO("get shm");

    struct conf_mgr *p_shm = (struct conf_mgr*)pv;
    return p_shm;
}


int main(void)
{
	init_shm();
	if(SIG_ERR == signal(SIGINT,fa)){
		perror("signal"),exit(-1);
	}
	while(1)
	{
		p = get_shm(shmid);
		if(NULL != p){
			int a = getchar();
			if(a != '\n'){
				DEBUG_INFO("%d",p->print_switch);
				DEBUG_INFO("%d",p->log_switch);
			}
		}
	}
	return 0;
}
