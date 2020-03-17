#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*全局函数构造，会在进入main之前调用*/
void fun1(void)__attribute__((constructor));

void fun1(void)
{
	printf("before main excute\n");
}

int main(void)
{
	printf("main mid\n");
	return 0;
}
