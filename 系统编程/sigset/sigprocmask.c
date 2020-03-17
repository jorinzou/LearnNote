//信号的屏蔽与恢复
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

void fa(int signum)
{
	DEBUG_INFO("%d",signum);
}

/*信号的屏蔽与恢复*/
void signal_mask(void)
{
	if (SIG_ERR == signal(SIGINT,fa)){
		perror("signal");
		exit(-1);
	}
	
	if (SIG_ERR == signal(SIGQUIT,fa)){
		perror("signal");
		exit(-1);
	}
	
	sigset_t set,oldset;
	
	/*清空信号量集*/
	if(-1 == sigemptyset(&set)){
		perror("sigemptyset"),exit(-1);
	}
	if(-1 == sigemptyset(&oldset)){
		perror("sigemptyset"),exit(-1);
	}
	
	/*把信号量集添加到集合中*/
	if(-1 == sigaddset(&set,2)){
		perror("sigaddset"),exit(-1);
	}
	
	/*把信号量集添加到集合中*/
	if(-1 == sigaddset(&set,3)){
		perror("sigaddset"),exit(-1);
	}
	
	int ret = sigprocmask(SIG_SETMASK,&set,&oldset);
	if(ret < 0){
		perror("sigprocmask");
		exit(-1);
	}
	
	//阻塞
	getchar();
	
	//获取信号屏蔽期间来过的信号
	sigset_t pend;
	ret = sigpending(&pend);
	if (-1 == ret){
		perror("sigpending");
		exit(-1);
	}
	
	//判断信号2是否存在于信号集set中
	if(1 == sigismember(&pend,2)){
		DEBUG_INFO("信号2存在");
	}
	
	//判断信号3是否存在于信号集set中
	if(1 == sigismember(&pend,3)){
		DEBUG_INFO("信号3存在");
	}
	
	/*恢复信号,恢复信号之后，之前屏蔽的信号行为，会被依次执行*/
	ret = sigprocmask(SIG_SETMASK,&oldset,NULL);
	if(ret < 0){
		perror("sigprocmask");
		exit(-1);
	}
}

int main(void)
{
	signal_mask();
	
	while(1)
	{
	   sleep(1);
	}
	return 0;
}






