//使用sbrk函数调整动态内存的大小
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	//使用sbrk函数申请int类型大小的动态内存
	//void* p1 = malloc(sizeof(int));
	void* p1 = sbrk(sizeof(int));
	if((void*)-1 == p1)
	{
		perror("sbrk"),exit(-1);
	}
	//按照上述方法再申请2次 p2 p3
	void* p2 = sbrk(sizeof(int));
	if((void*)-1 == p2)
	{
		perror("sbrk"),exit(-1);
	}
	void* p3 = sbrk(sizeof(int));
	if((void*)-1 == p3)
	{
		perror("sbrk"),exit(-1);
	}
	printf("p1 = %p,p2 = %p,p3 = %p\n",p1,p2,p3);
	
	printf("---------------------------\n");
	//获取当前动态内存的末尾地址
	void* cur = sbrk(0);
	printf("cur = %p\n",cur); // p3+4
	
	//释放4个字节的动态内存
	void* p5 = sbrk(-4);
	printf("p5 = %p\n",p5); // cur 

	printf("--------------------------\n");
	cur = sbrk(0);
	printf("cur = %p\n",cur); // p3

	void* p6 = sbrk(-4);
	printf("p6 = %p\n",p6); // p3

	cur = sbrk(0);
	printf("cur = %p\n",cur);// p2

	printf("----------------------------\n");
	printf("当前进程的编号是：%d\n",getpid());
	printf("当前进程拥有4个字节的动态内存\n");
	getchar();

	sbrk(4093);
	printf("申请4093个字节，此时正好超过了1个内存页的大小\n");
	getchar();

	sbrk(-1);
	printf("释放1个字节，此时正好剩下一个内存页的大小\n");
	getchar();

	sbrk(-4096);
	printf("释放完毕所有动态内存\n");
	getchar();
	return 0;
}







