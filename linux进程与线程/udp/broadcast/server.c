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

int broadcast_sock =-1;
#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)
void unicast_send(struct sockaddr_in *ipaddr);

//广播初始化
void broadcast_init(void)
{
    int ret = -1;
    int so_broadcast = 1;
	struct sockaddr_in server_addr;

    broadcast_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcast_sock < 0)
    {
        perror("create socket failed:");
		return;
    }
    memset((void*) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
	//广播发送or广播接收的地址段
    server_addr.sin_addr.s_addr = inet_addr("172.16.8.255");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(9898);
    ret = bind(broadcast_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (ret < 0)
    {
    	close(broadcast_sock);
        perror("bind error");
        return;
    }
    //设置套接字属性，以支持广播
    ret = setsockopt(broadcast_sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast,sizeof(so_broadcast));
	return;
}


//解析广播接收的数据
void broadcast_socket_recv(void)
{
	int i = 0;
	int n = 0;
	char macstring[17] = {0};
	unsigned char mac[8] = {0};
	struct sockaddr_in ipaddr; 
	int ret;
	int len = sizeof(ipaddr);
	uint8_t buf[2048] = {0};
	ret = recvfrom(broadcast_sock,buf,sizeof(buf),MSG_WAITALL,(struct sockaddr*)&ipaddr,&len);
	if (buf[0] == 0xfd) 
	{
		n = 1;
		for(i=0; i < 8; i++)
		{
			mac[i] = buf[n+i] & 0x000000ff;
		}
		sprintf(macstring,"%02x%02x%02x%02x%02x%02x%02x%02x",mac[7],mac[6],mac[5],mac[4],mac[3],mac[2],mac[1],mac[0]);
		DEBUG_INFO("%s",macstring);
		DEBUG_INFO("ipaddr = %s",inet_ntoa(ipaddr.sin_addr));
		//获得广播发送方的源地址后，单播回复
		unicast_send(&ipaddr);
	}
}

//阻塞等待
void * broadcast_poll(void *data)
{
	while(1)
	{
		struct pollfd pollfds[1];
		pollfds[0].fd=broadcast_sock;
		pollfds[0].events=POLLIN;
		poll(pollfds,1,-1);
		if(pollfds[0].revents)
		{
			broadcast_socket_recv();
		}
	}
	return NULL;
}

//单播发送
void unicast_send(struct sockaddr_in *ipaddr)
{
	char buf[] = {"dan send broadcast"};
	int ret = 0;
	ret = sendto(broadcast_sock,buf,strlen(buf),0,(struct sockaddr*)ipaddr,sizeof(*ipaddr));
	if (ret < 0)
	{
		perror("sendto failed");
		DEBUG_INFO("ret=%d",ret);
	}
	DEBUG_INFO("unicast send");
}

int main(void)
{
	broadcast_init();
	//广播接收线程
	pthread_t recv_id;
	pthread_create(&recv_id,NULL,broadcast_poll,NULL);

	while(1)
	{
		sleep(1);
	}
	return 0;
}