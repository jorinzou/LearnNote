#include <stdio.h>

/*
内存地址有高代之分，数据大小有高位和低位之分
如果这个数据在内存中存放是，数据从代位到高位，对应内存地址的低位到高位，那么是小端
如果数据的高位对应内存地址的低位地址，低位对应内存地址的高地址，就是小端模式
*/

typedef union {
	int a;
	unsigned char b[4];
}A;

int main(void)
{
	A t = {0x98765432};
	printf("0x%x\n",t.a);
	int i = 0;
	for(i; i < 4; ++i){
		printf("%p=0x%x\n",&t.b[i],t.b[i]);
	}
	return 0;
}
