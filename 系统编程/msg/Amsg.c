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

void trim_string(char *str)
{
	int len = strlen(str);
	//去掉最后的换行符
	if(str[len-1] == '\n'){
		len--;		//字符串长度减一
		str[len] = 0;	//给字符串最后一个字符赋0，即结束符
	}
}

void fa(int signum)
{
	sleep(1);
	//调用msgctl函数实现删除
	int res = msgctl(msqid,IPC_RMID/*删除标记*/,NULL);
	if(-1 == res){
		perror("msgctl"),exit(-1);
	}
	DEBUG_INFO("删除消息队列成功\n");
	exit(0);
}

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
	Msg msg;
	memset(&msg,0,sizeof(msg));
	init_msg();
	while(1){
		scanf("%d",&msg.mtype);
		DEBUG_INFO("%d",msg.mtype);
		//此函数会阻塞
		int res = msgrcv(msqid,&msg,sizeof(msg.buf),msg.mtype/*消息类型*/,0);
		if(-1 == res){
			perror("msgrcv"),exit(-1);
		}
		DEBUG_INFO("msg type：%ld,msg buf：%s",msg.mtype,msg.buf);
		memset(&msg,0,sizeof(msg));
	}
	return 0;
}