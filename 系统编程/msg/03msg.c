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
#define MSG_LEN 8184
typedef struct 
{
	long mtype;//消息的类型 64bit system 8byte
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

//初始化并创建消息队列
void init_msg(void)
{
	key_t key = ftok(".",150);
	if(-1 == key){
		perror("ftok"),exit(-1);
	}
	DEBUG_INFO("key = %#x",key);
	//创建消息队列，使用msgget函数；不存在则创建，存在则打开IPC_CREAT|IPC_EXCL
	msqid = msgget(key,0/*0表示获取已存在的消息队列*/);
	if(-1 == msqid){
		perror("msgget"),exit(-1);
	}
	DEBUG_INFO("msqid = %d",msqid);
}

int main(void)
{
	init_msg();
	Msg msg;
	int a = 1;
	while(1){
		/*消息类型必须大于0*/
		//scanf("%d",&msg.mtype);
		msg.mtype = a++;
		sleep(1);
		DEBUG_INFO("type:%d",msg.mtype);
		memset(msg.buf,msg.mtype,sizeof(msg.buf));
		int res = msgsnd(msqid,&msg,sizeof(msg),0/*这个参数默认是给0*/);
		if(-1 == res){
			perror("msgsnd");
			continue;
		}
		DEBUG_INFO("send type:%d",msg.mtype);
	}
	return 0;
}
