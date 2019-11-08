#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int  sockefd;

void socket_init(void)
{
    struct sockaddr_in clivaddr;
    
     /* 创建 socket */
    if ((sockefd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //初始化服务器的ip地址与端口
    memset(&clivaddr, 0, sizeof(clivaddr));
    clivaddr.sin_family = AF_INET;
    clivaddr.sin_addr.s_addr = inet_addr("172.16.8.168");
    clivaddr.sin_port = htons(6868);

    /* connect server  */
    if (connect(sockefd, (struct sockaddr *)&clivaddr, sizeof(clivaddr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void* input_poll(void *arg)
{
    struct pollfd poll_fd[2];
    poll_fd[0].fd = 0;
    poll_fd[0].events = POLLIN;
	poll_fd[1].fd = sockefd;
    poll_fd[1].events = POLLIN;
    int i = 0;
    int ret = -1;

    while(1) {
        int ret = poll(poll_fd,2,-1);
        if (ret < 0){
            perror("poll");
            continue;
        }
         
        if (ret == 0){
            DEBUG_INFO("poll timeout!\n");
            continue;
        }

        if (poll_fd[0].revents == POLLIN){
            char buf[2048] = {0};
            //文件描述符存在(0、１、2), 0与进程的标准输入相关联，
            //１与进程的标准输出相关联，2与进程的标准错误输出相关联
            ret = read(0,buf,sizeof(buf)-1);
			buf[ret-1] = '\0';//去掉最后回车
            ret = send(sockefd,buf,strlen(buf),0);
            DEBUG_INFO("client send len=%d,send buf:%s",ret,buf);
         }
		if (poll_fd[1].revents == POLLIN){
			unsigned char recv_buf[128] = {0};
			ret = recvfrom(sockefd,&recv_buf,sizeof(recv_buf),MSG_DONTWAIT/*非阻塞*/,NULL,0); 
			DEBUG_INFO("client recv len=%d,recv_buf=%s",ret,recv_buf);
		}
    }
}

int main(void)
{
    int ret = 0;	
    socket_init();
    pthread_t input_id;
    pthread_create(&input_id, NULL, &input_poll,NULL);

    while(1) {
        sleep(1);
    }
	return 0;
}

