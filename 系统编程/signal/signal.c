#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/syscall.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%d]\033[0m "#fmt"\r\n",__FUNCTION__, __LINE__, ##args)

void fa(int signum)
{
	if(signum == SIGINT){
		DEBUG_INFO("%d",SIGINT);
	}
	if (signum == SIGQUIT){
		DEBUG_INFO("%d",SIGQUIT);
	}
}

//获取进程id
void get_process_pid(const char *function)
{
    //用于查找进程id，可以使用strace工具跟踪系统调用，当cpu卡住时,定位问题
    char msg[1024] = {0};
    sprintf(msg,"%s,pid:%d",function,syscall(SYS_gettid));
	DEBUG_INFO("%d",msg);
}

//捕获coredump信号
void coredump_signal(int signum)
{
	if(signum == SIGSEGV){
		DEBUG_INFO("%d",SIGSEGV);
	}
	DEBUG_INFO("%d",syscall(SYS_gettid));
	sleep(1);
}

void *process(void *arg)
{
	//pthread_detach(pthread_self());
	//每2scoredump一次
	while(1){
		sleep(1);
		DEBUG_INFO("child %d",syscall(SYS_gettid));
		int *p = NULL;
		*p = 123;
	}
}

int main(void)
{
	if(SIG_ERR == signal(SIGINT,fa)){
		perror("signal");
		exit(-1);
	}
	
	if(SIG_ERR == signal(SIGQUIT,fa)){
		perror("signal");
		exit(-1);
	}
	
	#if 0//SIGKILL信号只能用默认处理方式
	if(SIG_ERR == signal(SIGKILL,fa)){
		perror("signal");
		exit(-1);
	}
	#endif
	
	//重定义coredump信号处理方式
	if(SIG_ERR == signal(SIGSEGV,coredump_signal)){
		perror("signal");
		exit(-1);
	}
	
	pthread_t t;
	pthread_create(&t,NULL,process,NULL);
	
	DEBUG_INFO("%d",syscall(SYS_gettid));
	while(1){
		sleep(1);
		//DEBUG_INFO("main");
		DEBUG_INFO("main %d",syscall(SYS_gettid));
	}
	return 0;
}