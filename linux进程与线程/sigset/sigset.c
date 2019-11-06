//信号集的基本概念和使用
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

void print_set(void);
sigset_t set;
/*信号量集的基本使用*/
void sigset_use(void)
{
	// 128个字节
	DEBUG_INFO("sizeof(set) = %d  ",sizeof(set));
	//DEBUG_INFO("set = %d  ",set); //随机数
	DEBUG_INFO("set = %lu  ",set.__val[0]);
	
	DEBUG_INFO("----------------------------");
	//清空信号集set，打印
	if(-1 == sigemptyset(&set))
	{
		perror("sigemptyset"),exit(-1);
	}
	DEBUG_INFO("set = %lu  ",set.__val[0]);// 0
	//增加信号2到信号集set中，打印
	if(-1 == sigaddset(&set,2))
	{
		perror("sigaddset"),exit(-1);
	}
	DEBUG_INFO("set = %lu  ",set.__val[0]); // 2
	print_set();
	//增加信号3到信号集set中，打印
	if(-1 == sigaddset(&set,3))
	{
		perror("sigaddset"),exit(-1);
	}
	DEBUG_INFO("set = %lu  ",set.__val[0]);// 6
	//增加信号7到信号集set中，打印
	if(-1 == sigaddset(&set,7))
	{
		perror("sigaddset"),exit(-1);
	}
	DEBUG_INFO("set = %lu  ",set.__val[0]);// 70
	
	DEBUG_INFO("----------------------------");
	//从信号集set中删除信号3，打印
	if(-1 == sigdelset(&set,3))
	{
		perror("sigdelset"),exit(-1);
	}
	DEBUG_INFO("set = %lu  ",set.__val[0]); // 66
	//判断信号2是否存在于信号集set中
	if(1 == sigismember(&set,2))
	{
		DEBUG_INFO("信号2存在  ");
	}
	//判断信号3是否存在于信号集set中
	if(1 == sigismember(&set,3))
	{
		DEBUG_INFO("信号3存在  ");
	}
	//判断信号7是否存在于信号集set中
	if(1 == sigismember(&set,7))
	{
		DEBUG_INFO("信号7存在  ");
	}

	DEBUG_INFO("------------------------");
	// 填满信号集
	if(-1 == sigfillset(&set))
	{
		perror("sigfillset"),exit(-1);
	}
	// 很大的数
}

void print_set(void)
{
	int i = 0;
	for(i=0; i < sizeof(set)/sizeof(set.__val[0]); i++){
		DEBUG_INFO("%lu",set.__val[i]);
	}
}

int main(void)
{
	#if 0
    DEBUG_INFO("%d",sizeof(unsigned long int));	
	unsigned long int num = 0xFFFFFFFFFFFFFFFF;
	DEBUG_INFO("%lu",num);
	unsigned long int a = 18446744067267100671ul;//64位系统才能放这么大的数
	DEBUG_INFO("0x%x",a);
	#endif
	sigset_use();
	return 0;
}






