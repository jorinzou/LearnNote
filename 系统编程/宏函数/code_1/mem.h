#ifndef _MEM_H_
#define _MEM_H_

//声明自定义malloc及free函数
extern void *my_malloc(unsigned int uSize, const char *pszFunc, unsigned int uLine);
extern void my_free(void *pPtr, const char *pszFunc, unsigned int uLine);

//重定义malloc及free
#define malloc(size) my_malloc(size, __FUNCTION__, __LINE__)
#define free(ptr) my_free(ptr, __FUNCTION__, __LINE__);

#endif