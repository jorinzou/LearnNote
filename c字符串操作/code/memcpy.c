#include <string.h>
#include <stdio.h>
#include <assert.h>

//字符串的memcpy实现
void *m_memcpy(void *dest,const void *src,size_t n)
{
		assert(dest != NULL);
		assert(src != NULL);
		char *tmp = dest;
		const char *s = src;
		while(n--){
			*tmp++ = *s++;
		}
		return dest;
}

int main(void)
{
	char dest[12] = {0};
	char src[12] = {"test"};
	m_memcpy(dest,src,4);
	
	return 0;
}
