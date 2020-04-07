#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

int main(void)
{
	int *p = malloc(4);
	*p = 123;
	printf("%d\n",*p);
	return 0;
}
