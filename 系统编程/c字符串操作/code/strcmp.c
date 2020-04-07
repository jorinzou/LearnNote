#include <string.h>
#include <stdio.h>
#include <assert.h>

int m_strcmp1(const char *src,const char *dst)
{
	assert(src != NULL || dst != NULL);
	unsigned char c1;
	unsigned char c2;
	do{
		c1 = (unsigned char)*src++;
		c2 = (unsigned char)*dst++;
		if (c1 == '\0'){	
			return c1 - c2;
		}
	}while(c1 == c2);
	return c1 - c2;
}

int m_strcmp2(const char *s1,const char *s2)
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

	printf("%d\n",m_strcmp1(s1,s2));
	
	return 0;
}
