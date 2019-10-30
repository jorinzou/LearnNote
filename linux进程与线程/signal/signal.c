#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <error.h>
#include <time.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

void fa(int signum)
{
	if(signum == SIGINT){
		DEBUG_INFO("%d",SIGINT);
	}
	if (signum == SIGQUIT){
		DEBUG_INFO("%d",SIGQUIT);
	}
	if (signum == SIGKILL){
		DEBUG_INFO("%d",SIGKILL);
	}
}

int main(void)
{
	int ret = signal(SIGINT,fa);
	if(ret < 0){
		//DEBUG_INFO("%s",strerror(ret));
	}
	
	ret = signal(SIGQUIT,fa);
	if(ret < 0){
		//DEBUG_INFO("%s",strerror(ret));
	}
	
	ret = signal(SIGKILL,fa);
	if(ret < 0){
		//DEBUG_INFO("%s",strerror(ret));
	}
	
	while(1)
	{
		sleep(1);
	}
	return 0;
}