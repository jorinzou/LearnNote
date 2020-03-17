#include <stdio.h>

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
