#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int  sockefd;

void socket_init(void)
{
    struct sockaddr_in servaddr;
    
     /* 创建 socket */
    if ((sockefd = socket(AF_INET, SOCK_STREAM/*tcp*/, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //初始化服务器的ip地址与端口
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);

	int reuse = 0;
	setsockopt(sockefd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    /* 绑定地址到 socket */
    if (bind(sockefd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* 转为监听 socket */
    if (listen(sockefd, 5)) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
	 int ret = 0;
	 socket_init();
	 struct sockaddr_in cliaddr;
	 socklen_t   clilen = sizeof(cliaddr);
	 int connfd = accept(sockefd, (struct sockaddr *)&cliaddr, &clilen);
	 if (connfd == -1) {
		perror("accept");
		return -1;
	 }

	 /* 设置成非阻塞模式
	 if (fcntl(connfd,F_SETFL,O_NONBLOCK) < 0){
		perror("fcntl");
		return -1;
	 }*/

	 DEBUG_INFO("connfd:%d",connfd);
	 DEBUG_INFO("%s",inet_ntoa(cliaddr.sin_addr));

	 unsigned char recv_buf[32] = {0};
	 while(1) {
		ret = recvfrom(connfd,recv_buf,sizeof(recv_buf),MSG_DONTWAIT,NULL,0);
		if (ret < 0){
			//客户端异常断开，recv收到rst
			if (errno == ECONNRESET){
				DEBUG_INFO("rst");
				break;
			}
			else if (errno == EAGAIN){
				DEBUG_INFO("缓冲区无数据");
				sleep(2);
				continue;
			}
			else if (errno == EINTR){
				DEBUG_INFO("被信号中断");
				break;
			}
			else {
				DEBUG_INFO("recv failed");
				break;
			}
		}

		else if (ret == 0){
			//ctrl + c 终止进程
			//对端close(fd)
			DEBUG_INFO("连接被对端关闭");
			break;
		}
		
		else {
			DEBUG_INFO("server recv ret=%d,recv data=%s",ret,recv_buf);
			static int count = 1;
			count++;
			sendto(connfd,&count,sizeof(count),MSG_DONTWAIT,NULL,0);
			continue;
		}
	 }

	close(connfd);
	getchar();
	close(sockefd);
	
	return 0;
}

