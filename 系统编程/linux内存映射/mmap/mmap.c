#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FILE_SIZE 100*1024*1024

void file_mmap(void)
{
	int fd = open("./test.txt", O_RDWR|O_CREAT|O_TRUNC, 0664);
	if (fd < 0) {
		perror("open failed");
		return;
	}
	
	ftruncate(fd,FILE_SIZE);
	close(fd);
	fd = open("./test.txt",O_RDWR);
	char *mfd = mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (NULL == mfd) {
		perror("mmap");
		return;
	}
	close(fd);

	char buf[1025] = {"test"};
	int len = FILE_SIZE;
	char *p = mfd;
	while(len > 0){
		memcpy(p,buf,strlen(buf));
		p+=strlen(buf);
		len-=strlen(buf);
	}
	printf("end\n");
	getchar();
	munmap(mfd, FILE_SIZE);
}

int main(void)
{
	file_mmap();
	return 0;
}
