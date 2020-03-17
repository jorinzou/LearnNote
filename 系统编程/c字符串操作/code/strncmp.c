#include <string.h>
#include <stdio.h>
#include <assert.h>

int m_strncmp(const char *s1,const char *s2,size_t n)
{
		assert(s1 != NULL && s2 != NULL);
		while(n--){
			if (*s1 == '\0' || *s2 == '\0' || *s1++ != *s2++){
				return (*s1-*s2)>0?1:-1;
			}
		}

		return 0;
}

int main(void)
{
	char s1[10] = {"test2345"};
	char s2[12] = {"test234"};

	printf("%d\n",m_strncmp(s1,s2,8));
	
	return 0;
}
