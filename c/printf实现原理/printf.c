#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/*
1.几个宏介绍
va_list  原型:(char *)
va_start(x,y),把x指针指向y参数
va_arg(a,b),通过a，取得指向栈的下一个数据元素位置的指针 
va_copy(ap2, ap),ap2赋值为ap,
va_end(ap),把ap赋值为NULL

2.数据结构栈的原理 
先进后出
在内存布局中，地址从上到下，是从大到小

3.函数调用形参在栈中的分布
看图示

4.结合1，2，3就能依次取出printf形参在压在栈中的变量
*/

int fun(int num_args,...)
{
	va_list ap;
	va_start(ap,num_args);
	int i = 0;
	for(i; i < num_args; i++){
		int val = va_arg(ap,int);
		printf("%d\n",val);
	}
	va_end(ap);
}

int main(void)
{
	int b = 2;
	int c = 3;
	int d = 4;
	fun(3,b,c,d);
	return 0;
}
