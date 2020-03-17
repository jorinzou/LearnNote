#include <string.h>
#include <stdio.h>
#include <assert.h>

int m_strcmp(const char *s1,const char *s2)
{
		assert(s1 != NULL && s2 != NULL);
		while(*s1 != '\0' || *s2 != '\0'){
				if (*s1++ == *s2++){
					continue;
				}
				else{
					return (*s1++ - *s2++)>0?1:-1 ;
				}
		}
		return 0;
}

int main(void)
{
	char s1[10] = {"test235"};
	char s2[12] = {"test234"};

	printf("%d\n",m_strcmp(s1,s2));
	
	return 0;
}
