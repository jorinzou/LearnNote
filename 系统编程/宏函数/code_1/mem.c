#include <stdio.h>
#include <stdlib.h>

void *my_malloc(unsigned int uSize, const char *pszFunc, unsigned int uLine)
{
     printf("MALLOC: func:%s; line:%d/r/n", pszFunc, uLine);
     return malloc(uSize);   //调用标准C库的malloc.
}

void my_free(void *pPtr, const char *pszFunc, unsigned int uLine)
{
     printf("FREE: func:%s; line:%d/r/n", pszFunc, uLine);
     free(pPtr);   //调用标准C库的free.
}