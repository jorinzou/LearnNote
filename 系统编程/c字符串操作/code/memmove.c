#include <stdio.h>
#include <string.h>
#include <assert.h>

void *m_memmove(void *dst,const void *src,size_t n)
{
	assert(dst!=NULL && src!=NULL);
	char *tmpdst = dst;
	const char *tmpsrc = src;

	if(tmpdst+n < tmpsrc || tmpsrc+n < tmpdst){
		while(n--){
			*tmpdst++ = *tmpsrc++;
		}
	}
	else{
		tmpdst = tmpdst+n-1;
		tmpsrc = tmpsrc+n-1;
		while(n--){
			*tmpdst-- = *tmpsrc--;
		}
	}
	return dst;
}

int main(void)
{
	char s1[] = {"test memmove"};
	char s2[10] = {};

	m_memmove(s2,s1,6);
	printf("s1=%s\n,s2=%s\n",s1,s2);
	return 0;
}	
