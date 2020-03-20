#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//子进程直接退出,变成僵尸进程
void fork_child(void)
{
	while(1){
		sleep(1);
		exit(100);
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
	int status = -1;
	while(1){
		//加了wait之后,在/proc/pid 子进程的内存被回收
		pid = wait(&status);
		printf("pid = %d,status=%d\n",pid,status);
		scanf("%d",&status);
		exit(1);
	}
	return 0;
}
