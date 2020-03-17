#include <string.h>
#include <stdio.h>
#include <assert.h>

char *m_strncpy(char *dest,const char *src, size_t n)
{
		assert(dest != NULL && src != NULL);
		char *tmp = dest;
		while(*src!= '\0' && n--){
			*tmp++ = *src++;
		}
		return dest;
}

int main(void)
{
	char dest[10] = {0};
	char src[5] = {"123"};
	m_strncpy(dest,src,sizeof(dest) - 1);
	printf("dest=%s\n",dest);
	return 0;

}
