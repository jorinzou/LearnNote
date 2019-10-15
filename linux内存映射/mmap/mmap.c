#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void file_mmap(void)
{
	int fd = open("./test.txt", O_RDWR|O_CREAT, 0664);
	if (fd < 0) {
		perror("open failed");
		return;
	}

	//把文件大小限制为5000个字节
	ftruncate(fd, 5000);

	char *p = mmap(NULL, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (NULL == p) {
		perror("mmap");
		return;
	}
	close(fd);
	printf("%s\n",p);
	#if 1
	*(++p) = 'b';
	*(++p) = 'c';
	*(++p) = 'd';
	#endif
	printf("%s\n",p);
	munmap(p, 4*1024);
}

int main(void)
{
	file_mmap();
	return 0;
}
