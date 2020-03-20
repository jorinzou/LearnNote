#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void fork_child(void)
{
	pid_t pid = setsid();
	printf("pid=%d\n",pid);
	while(1){
		sleep(1);
	}
}

int main(void)
{
	pid_t pid = fork();
	if(pid == 0){
		fork_child();
	}

	printf("child pid:%d\n",pid);
	printf("parent pid:%d\n",getpid());
	int status;
	while(1){
		scanf("%d",&status);
		exit(1);
	}
	return 0;
}
