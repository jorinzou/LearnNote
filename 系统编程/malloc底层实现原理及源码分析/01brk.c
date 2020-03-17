//使用brk函数调整动态内存的大小
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	//使用sbrk(0)获取当前动态内存的末尾地址
	//既是末尾地址也是起始位置
	void* cur = sbrk(0);
	if((void*)-1 == cur)
	{
		perror("sbrk"),exit(-1);
	}
	printf("cur = %p\n",cur);
	//使用brk函数申请4个字节动态内存
	int res = brk(cur+4);
	if(-1 == res)
	{
		perror("brk"),exit(-1);
	}
	void* pv = sbrk(0);
	if((void*)-1 == pv)
	{
		perror("sbrk"),exit(-1);
	}
	printf("pv = %p\n",pv); // cur + 4

	printf("-------------------------\n");
	// 申请4个字节
	brk(cur+8);
	pv = sbrk(0);
	printf("pv = %p\n",pv); // pv+4 cur+8
	
	printf("--------------------------\n");
	// 释放所有动态内存
	brk(cur);
	pv = sbrk(0);
	printf("pv = %p\n",pv); // cur
	return 0;
}







