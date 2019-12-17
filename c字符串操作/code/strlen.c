#include <string.h>
#include <stdio.h>
#include <assert.h>

size_t m_strlen(const char *src)
{
	assert(src != NULL);
	size_t n = 0;
	while(*src++ != '\0'){
		n++;
	}
	return n;
}


int main(void)
{
	char src[12] = {"1234567890"};
	printf("src len=%d\n",m_strlen(src));

	return 0;
}


