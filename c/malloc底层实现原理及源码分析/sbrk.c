#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int *p = sbrk(4);
	printf("p = %p\n",p);
	*p = 12;
	int *p3 = sbrk(0);
	printf("p3=%p\n",p3);
	int *p2 = sbrk(-4);
	printf("p2=%p\n",p2);
	return 0;
}
