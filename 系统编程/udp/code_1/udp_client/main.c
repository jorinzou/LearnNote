#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>

#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)

int udp_sockfd = -1;

int connect_init(const char *unix_socket_path)
{
   struct sockaddr_un addr;
   addr.sun_family = AF_LOCAL;
   strncpy(addr.sun_path,unix_socket_path,sizeof(addr.sun_path)-1);
   DEBUG_INFO("%s",addr.sun_path);
   //unlink(unix_socket_path); 
   int ret = connect(udp_sockfd,(struct sockaddr*)&addr,sizeof(addr));
   if (ret < 0){
        perror("connect");
        DEBUG_INFO("connect failed");
        return -1;
   }
   DEBUG_INFO("connect sucess");
}


void *udp_send_thread(void *arg)
{
    unsigned char buf[] = {"udp client send data"};
    while(1){
        int cmd = 0;
        scanf("%d",&cmd);
        if (cmd == 1){
           int ret = connect_init("/tmp/udp_local_server");
           if (ret < 0){
               DEBUG_INFO("connect_init failed");
               continue;
           }
           ret = write(udp_sockfd,buf,sizeof(buf)); 
           if(ret < 0){
               perror("write failed");
               DEBUG_INFO("write error");
               continue;
           }
        }
    }
}

void *udp_recv_thread(void *arg)
{
    struct pollfd fds[1];
    fds[0].fd = udp_sockfd;
    fds[0].events = POLLIN;
    unsigned char buf[1024] = {0};
    while(1){
        poll(fds, 1, -1);
        if (fds[0].revents & POLLIN){
            int ret = read(udp_sockfd,buf,sizeof(buf));
            if (ret < 0){
                perror("read");
                DEBUG_INFO("read failed");
            }
            DEBUG_INFO("buf=%s",buf);
        }
    }
}

int udp_sock_init(const char *unix_socket_path)
{
    int sockfd = socket(AF_LOCAL,SOCK_DGRAM,0);
    if (sockfd < 0){
        perror("socket");
        DEBUG_INFO("socket error");
        return -1;
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    /*struct sockaddr_un {
	__kernel_sa_family_t sun_family;  AF_UNIX 
	char sun_path[UNIX_PATH_MAX];	 pathname 
	#define UNIX_PATH_MAX	108
    };*/
   struct sockaddr_un addr;
   addr.sun_family = AF_LOCAL;
   strncpy(addr.sun_path,unix_socket_path,sizeof(addr.sun_path)-1);
   DEBUG_INFO("%s",addr.sun_path);
   unlink(unix_socket_path); 
   int tr = 1;
   int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));
   if (ret < 0){
        perror("setsockopt");
        DEBUG_INFO("setsockopt error");
   }
   ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
   if (ret < 0){
        perror("bind");
        DEBUG_INFO("bind failed");
        return -1;
   }
   DEBUG_INFO("ret=%d,sockfd=%d",ret,sockfd);
   return sockfd;
}

int main(void)
{
    udp_sockfd = udp_sock_init("/tmp/udp_local_client");
    if (udp_sockfd < 0){
        DEBUG_INFO("udp_sock_init failed");
        return -1;
    }
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1,NULL,udp_send_thread,NULL);
    pthread_create(&t2,NULL,udp_recv_thread,NULL);

    while(1){
        sleep(1);
    }

    return 0;
}


