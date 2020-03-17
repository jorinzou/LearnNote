#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
1.后台用strace工具跟踪系统调用，fwrite底层调用的是write
带缓冲，写满4096之后，再同步到硬盘
2.write是马上写到硬盘，效率比fwrite低
*/

void fwrite_test(void)
{
	unsigned long long i = 10000000;
	FILE *fp = NULL;
	fp = fopen("./fwrite.dat","w+");
	if (fp == NULL){
		return;
	}

	char buf[10] = {"test"};
	time_t t1 = time(NULL);
	while(i--)
	{
		fwrite(buf,1,strlen(buf),fp);
	}
	fclose(fp);
	time_t t2 = time(NULL);
	printf("fwrite time = %d\n",t2-t1);
}

void write_test(void)
{
	unsigned long long i = 10000000;
	int fp = open("./write.dat",O_RDWR|O_CREAT|O_TRUNC,0666);
	if (fp <0){
		printf("fp=%d\n",fp);
		return;
	}
	
	time_t t1 = time(NULL);
	char buf[12] = "test";
	while(i--){
		write(fp,buf,strlen(buf));
	}
	close(fp);
	time_t t2 = time(NULL);
	printf("write time:%d\n",t2-t1);
}

int main(void)
{
	fwrite_test();
	write_test();
	return 0;
}
