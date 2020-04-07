#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma back(1)

//取消结构体对齐
#if 1
typedef struct data1{
	int a;
	char b;
}__attribute__((packed))data1_t;
#endif

//指定结构体对齐
#if 1
typedef struct data2{
	int a;
	char b;
}__attribute__((aligned(4)))data2_t;
#endif

int main(void)
{
	printf("%d\n",sizeof(data1_t));
	printf("%d\n",sizeof(data2_t));
	return 0;
}
