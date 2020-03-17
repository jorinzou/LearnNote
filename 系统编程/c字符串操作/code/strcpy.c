#include <string.h>
#include <stdio.h>
#include <assert.h>

char *m_strcpy(char *dest,const char *src)
{
		assert(dest != NULL && src != NULL);
		char *tmp = dest;
		while ((*tmp++ = *src++) != '\0');
		return dest;
}

int main(void)
{
	char dest[10] = {0};
	char src[10] = {"123456789"};
	m_strcpy(dest,src);
	printf("dest=%s\n",dest);
	return 0;

}
