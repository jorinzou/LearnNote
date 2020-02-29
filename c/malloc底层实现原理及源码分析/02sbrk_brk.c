//使用sbrk函数申请，brk函数释放
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	//1.申请int类型大小的内存，放数据66
	int* pi = (int*)sbrk(sizeof(int));
	if((int*)-1 == pi)
	{
		perror("sbrk"),exit(-1);
	}
	*pi = 66;
	//2.申请double类型大小内存，放数据3.14
	double* pd =(double*)sbrk(sizeof(double));
	if((double*)-1 == pd)
	{
		perror("sbrk"),exit(-1);
	}
	*pd = 3.14;
	//3.申请字符串大小内存，放数据"hello"
	char* pc = (char*)sbrk(10*sizeof(char));
	if((char*)-1 == pc)
	{
		perror("sbrk"),exit(-1);
	}
	//修改指针指向的内容
	strcpy(pc,"hello");
	//4.打印上述所有数据内容
	printf("*pi = %d,*pd = %g,pc = %s\n",*pi,*pd,pc);
	//5.释放所有动态内存，使用brk函数
	int res = brk((void*)pi);
	if(-1 == res)
	{
		perror("brk"),exit(-1);
	}
	printf("释放所有动态内存完毕\n");
	return 0;
}





