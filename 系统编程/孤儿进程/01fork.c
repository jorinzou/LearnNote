#include <stdio.h>
#include <unistd.h>

void fork_child(void)
{
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
	return 0;
}
