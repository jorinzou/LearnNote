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
#define MSG_LEN 1024
typedef struct 
{
	long mtype;//消息的类型
	char buf[MSG_LEN];//消息的内容
}Msg;

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
	int size = 0;
	DEBUG_INFO("Msg=%d",sizeof(Msg));
	while(1){
		/*消息类型必须大于0*/
		//scanf("%d",&msg.mtype);
		msg.mtype = a++;
		sleep(1);
		//发送消息,此函数不会阻塞
		memset(msg.buf,9,sizeof(msg.buf));
		int res = msgsnd(msqid,&msg,sizeof(msg),0/*这个参数默认是给0*/);
		if(res < 0){
			perror("msgsnd"),exit(-1);
			continue;
		}
		size+=res;
		DEBUG_INFO("send type:%d, size=%d",msg.mtype);
	}
	return 0;
}
