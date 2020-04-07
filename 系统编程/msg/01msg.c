#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int msqid = -1;

//接收可以随意定，发送时必须<=8192,内核头文件,ipc_namespace.h，定义的宏DFLT_MSGSIZEMAX为8192,可以修改这个并重新编译内核来改变大小
#define MSG_LEN 8192
typedef struct 
{
	long mtype;//消息的类型
	char buf[MSG_LEN];//消息的内容
}Msg;

//初始化并创建消息队列
void init_msg(void)
{
	key_t key = ftok(".",150);
	if(-1 == key)
	{
		perror("ftok"),exit(-1);
	}
	DEBUG_INFO("key = %#x",key);
	//创建消息队列，使用msgget函数；IPC_CREAT不存在则创建，存在则打开,IPC_EXCL存在则退出
	msqid = msgget(key,IPC_CREAT|0664);
	if(-1 == msqid){
		perror("msgget"),exit(-1);
	}
	DEBUG_INFO("msqid = %d",msqid);
}

int main(void)
{
	init_msg();
	while(1){
		sleep(1);
	}
	return 0;
}
