#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>
#include <string.h>
#include <poll.h>
#include <pthread.h>
#include <sys/signal.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <syslog.h>  
#include <pthread.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int broadcast_sock =-1;
struct sockaddr_in server_addr;

int broadcast_init(void)
{
    int ret = -1;
    int so_broadcast = 1;

    //建立数据报套接字
    broadcast_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcast_sock < 0)
    {
        perror("create socket failed:");
        return ;
    }
    memset((void*) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
	//指定广播地址or接收广播
    //server_addr.sin_addr.s_addr = inet_addr("172.16.8.255");
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9898);
    ret = bind(broadcast_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (ret < 0)
    {
    	close(broadcast_sock);
        perror("bind error");
        return;
    }
    //设置套接字属性用于支持广播发送，广播接收 and so on.
    ret = setsockopt(broadcast_sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast,sizeof(so_broadcast));
	return;
}

//接收广播包or单播包
void* broadcast_recv(void *arg)
{
	int len=0;
	int ret;
	uint8_t buf[1024] = {0};
	struct sockaddr_in ipaddr; 
	struct pollfd pollFd;
	pollFd.fd = broadcast_sock;
	pollFd.events = POLLIN;

	while(1)
	{
		poll(&pollFd,1,-1);
		if(pollFd.revents)
		{
			ret = recvfrom(broadcast_sock,buf,sizeof(buf),MSG_WAITALL,(struct sockaddr *)&ipaddr,&len);
			char *str_ipaddr = inet_ntoa(ipaddr.sin_addr);
			//过滤掉自己发送自己的广播包
			#if 1
			if (!strcmp(str_ipaddr,"172.16.8.207") || !strcmp(str_ipaddr,"127.0.0.1"))
				return;
			#endif
			DEBUG_INFO("%s",buf);
			DEBUG_INFO("%s",str_ipaddr);
		}
	}
	return NULL;
}

void * broadcast_send(void *data)
{
	unsigned char buf[] = {0xfd,0x2d,0x05,0x52,0xfe,0xff,0x6f,0x0d,0x00};
	int ret = -1;
	while(1)
	{
		ret = sendto(broadcast_sock,buf,sizeof(buf),0,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
		sleep(3);
	}
}

int main(void)
{
	broadcast_init();
	pthread_t send_id;
	pthread_t recv_id;
	//广播发送线程
	pthread_create(&send_id,NULL,broadcast_send,NULL);
	//接收广播包or单播包线程
	pthread_create(&recv_id,NULL,broadcast_recv,NULL);
	while(1)
	{
		sleep(1);
	}
	return 0;
}