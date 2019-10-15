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

int  sockefd;

void socket_init(void)
{
    struct sockaddr_in servaddr;
    
     /* 创建 socket */
    if ((sockefd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //初始化服务器的ip地址与端口
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

	int reuse = 0;
	setsockopt(sockefd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    /* 绑定地址到 socket */
    if (bind(sockefd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* 转为监听 socket */
    if (listen(sockefd, 1)) {
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
	 if (connfd == -1) 
	 {
			perror("accept");
			return -1;
	 }

	 #if 1
	 if (fcntl(connfd,F_SETFL,O_NONBLOCK) < 0)
	 {
		perror("fcntl");
		return -1;
	 }
	 #endif

   fprintf(stderr, "accept %d\n", connfd);
	 printf("%s\n",inet_ntoa(cliaddr.sin_addr));

	 unsigned char recv_buf[32] = {0};
	 while(1)
	 {
	 	sleep(1);
		ret = read(connfd,recv_buf,sizeof(recv_buf));
		if (ret < 0)
		{
			//客户端异常断开，recv收到rst
			if (errno == ECONNRESET)
			{
				printf("rst\n");
				break;
			}

			else if (errno == EAGAIN)
			{
				printf("缓冲区无数据\n");
				sleep(2);
				//break;
			}

			else if (errno == EINTR)
			{
				printf("被信号中断\n");
				break;
			}

			else 
			{
				printf("recv failed\n");
				break;
			}
		}

		else if (ret == 0)
		{
			//ctrl + c 终止进程
			//对端close(fd)
			printf("连接被对端关闭\n");
			break;
		}
		
		else 
		{
			printf("ret=%d,recv data=%s\n",ret,recv_buf);
			continue;
		}
	 }

	close(sockefd);
	close(connfd);
	return 0;
}

