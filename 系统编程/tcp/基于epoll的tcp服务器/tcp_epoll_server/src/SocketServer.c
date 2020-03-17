#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <syslog.h>
#include <sys/epoll.h>

#include "SocketServer.h"
#include "debug.h"

int epfd = -1;

SocketRecord_t *socketRecordHead = NULL;

int CreateSocketRecord(void)
{
	int tr = 1;
	SocketRecord_t *SrcRecord;
	SocketRecord_t *NewSocket = calloc(1 , sizeof(SocketRecord_t));
	NewSocket->ClientAddrLen = sizeof(NewSocket->ClientAddr);
	NewSocket->SocketFd = accept(socketRecordHead->SocketFd,(struct sockaddr *) &(NewSocket->ClientAddr), &(NewSocket->ClientAddrLen));
	if (NewSocket->SocketFd < 0) {
		DEBUG_INFO("ERROR on accept");
		free(NewSocket);
		return -1;
	}
    DEBUG_INFO("client %s connect",inet_ntoa(NewSocket->ClientAddr.sin_addr));
    
    AddEpollEvent(NewSocket->SocketFd);
    SetLinger(NewSocket->SocketFd,1);
	// "Address Already in Use" error on the bind
	setsockopt(NewSocket->SocketFd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));
	fcntl(NewSocket->SocketFd, F_SETFL, O_NONBLOCK);
	NewSocket->next = NULL;
	SrcRecord = socketRecordHead;
	while (SrcRecord->next)
		SrcRecord = SrcRecord->next;

	SrcRecord->next = NewSocket;
	return (NewSocket->SocketFd);
}

void DeleteSocketRecord(int RmSocketFd)
{
	SocketRecord_t *SrcRecord, *PrevRecord = NULL;
	SrcRecord = socketRecordHead;
	while ((SrcRecord->SocketFd != RmSocketFd) && (SrcRecord->next)) {
		PrevRecord = SrcRecord;
		SrcRecord = SrcRecord->next;
	}

	if (SrcRecord->SocketFd != RmSocketFd) {
		return;
	}

	if (SrcRecord) {
		if (PrevRecord == NULL) {
			return;
		}
		PrevRecord->next = SrcRecord->next;
        DelEpollEvent(SrcRecord->SocketFd);
        DEBUG_INFO("client %s close",inet_ntoa(SrcRecord->ClientAddr.sin_addr));
		close(SrcRecord->SocketFd);
		free(SrcRecord);
	}
}

void DelEpollEvent(int SockFd)
{
    struct epoll_event ev;
    ev.data.fd = SockFd;
    ev.events=EPOLLIN|EPOLLET|EPOLLHUP;
    epoll_ctl(epfd, EPOLL_CTL_DEL, SockFd, &ev);
}

/*events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，
如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里 */

void AddEpollEvent(int SockFd)
{
    struct epoll_event ev;
    ev.data.fd = SockFd;
    ev.events=EPOLLIN|EPOLLET|EPOLLHUP;
    epoll_ctl(epfd, EPOLL_CTL_ADD, SockFd, &ev);
}

/*
设置 l_onoff 为非0，l_linger为非0，当套接口关闭时内核将拖延一段时间（由l_linger决定）如果套接口缓冲区中仍残留数据，进程将处于睡眠状态，
直到有数据发送完且被对方确认，之后进行正常的终止序列。
此种情况下,应用程序查close的返回是非常重要的，如果在数据发送完并被确认前时间到
close将返回EWOULDBLOCK错误且套接口发缓冲区中的任何数据都丢失
close的成功返回仅告诉我们发的数据（和FIN）已由对方TCP确认
它并不能告诉我们对方应用进程是否已读了数据,如果套接口设为非阻塞的，它将不等待close完成
*/
void SetLinger(const int SockFd, unsigned l_onoff)
{
	int z; /* Status code*/	
	struct linger so_linger;

	so_linger.l_onoff = l_onoff;
	so_linger.l_linger = 30;
	
	z = setsockopt(SockFd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof( so_linger));
	if (z) {
		perror("setsockopt(2)");
	}
}

int SocketSeverInit(int port)
{
	struct sockaddr_in serv_addr;
	int stat, tr = 1;

	if (socketRecordHead == NULL) {	
		SocketRecord_t *lsSocket = malloc(sizeof(SocketRecord_t));
		lsSocket->SocketFd = socket(AF_INET, SOCK_STREAM, 0);
		if (lsSocket->SocketFd < 0) {
			DEBUG_INFO("ERROR opening socket");
			return -1;
		}
		setsockopt(lsSocket->SocketFd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));
		// Set the fd to none blocking
		fcntl(lsSocket->SocketFd, F_SETFL, O_NONBLOCK);
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);
		stat = bind(lsSocket->SocketFd, (struct sockaddr*) &serv_addr,sizeof(serv_addr));
		if (stat < 0) {
			DEBUG_INFO("ERROR on binding: %s\n", strerror(errno));
			return -1;
		}
		listen(lsSocket->SocketFd, 5);
		lsSocket->next = NULL;
		socketRecordHead = lsSocket;
        epfd = epoll_create(EPOLL_SIZE);
        if (epfd < 0)  {
           perror("epoll_create");
           exit(-1);
        }
        AddEpollEvent(lsSocket->SocketFd);
	}

	return 0;
}

void GetClientFds(int *fds, int maxFds)
{
	int recordCnt = 0;
	SocketRecord_t *SrcRecord;
	SrcRecord = socketRecordHead;
	while ((SrcRecord) && (recordCnt < maxFds)) {
		fds[recordCnt++] = SrcRecord->SocketFd;
		SrcRecord = SrcRecord->next;
	}
	return;
}

int GetNumClients(void)
{
	int recordCnt = 0;
	SocketRecord_t *SrcRecord;
	SrcRecord = socketRecordHead;
	if (SrcRecord == NULL) {
		return -1;
	}

	while (SrcRecord) {
		SrcRecord = SrcRecord->next;
		recordCnt++;
	}
	return (recordCnt);
}

SocketRecord_t * GetAuthBySockfd(int ClinetFd)
{
	SocketRecord_t * SrcRecord;
	SrcRecord = socketRecordHead;
	while (SrcRecord!=NULL) {
		if(SrcRecord->SocketFd == ClinetFd)
			return SrcRecord;
		else
			SrcRecord=SrcRecord->next;
	}
	return NULL;
}

void ReceiveParse(int ClinetFd)
{
        SocketRecord_t *SocketRecord=GetAuthBySockfd(ClinetFd);
        if (SocketRecord == NULL) {
            return;
        }

        int retry_time = 0;
        char recv_buf[1024] = {0};
        while(1){
             int ret = recvfrom(ClinetFd,recv_buf,sizeof(recv_buf),MSG_DONTWAIT,NULL,0);
             if (ret < 0) {
                /* 非阻塞模式下，EAGAIN表示缓冲区暂无数据 */
                if (errno == EAGAIN){
    				usleep(500); 
    				if( ++retry_time <= 100){
    				    /* 重读指定次数 */
    					continue; 
    				}
                     /* 超过重读指定次数则退出*/
    				break;       
			    }

                else if (errno == EINTR){
                    DEBUG_INFO("被信号中断");
                    return;
                }

                else{
                    DEBUG_INFO("error: recv failed ");
                    DeleteSocketRecord(ClinetFd);
    				return ;
    			}
             }

             else if (ret == 0){
                //客户端ctrl + c 终止进程
                //对端close(fd)
                DEBUG_INFO("连接被对端关闭");
                DeleteSocketRecord(ClinetFd);
                return;
            }

            else {
                DEBUG_INFO("server recv len=%d,recv data=%s",ret,recv_buf);
                static int count = 1;
                count++;
                unsigned char buf[128] = {0};
                sprintf(buf,"%d",count);
                SeverSend(buf,strlen(buf),ClinetFd);
           }
        }
}

void* SeverEpoll(void* userData)
{
	int nfds = -1;
	int i;
	struct epoll_event events[EVENT_SIZE];
   
	while(1) {
		/*-1永久阻塞*/
        nfds = epoll_wait(epfd, events, EVENT_SIZE, -1);
		for(i = 0; i < nfds; ++i) {
        	if (events[i].data.fd == socketRecordHead->SocketFd) {
                CreateSocketRecord();
			}
			else if(events[i].events & EPOLLIN) {
            	if (events[i].data.fd < 0) {
                    continue;
            	}
				ReceiveParse(events[i].data.fd);
			}
			else if(events[i].events & EPOLLHUP) {
                DEBUG_INFO("EPOLLHUP");
                DeleteSocketRecord(events[i].data.fd);
			}
			else if(events[i].events & EPOLLERR) {
				DEBUG_INFO("EPOLLERR");
			}
		}
	}
	
  return NULL;
}

int SeverSend(uint8_t* buf, int len, int ClientFd)
{
	int rtn;
	if (ClientFd) {
		int SendNum = 0;
		int SendLenth = len ;
		while(len) {
			rtn = sendto(ClientFd,buf,len,MSG_DONTWAIT,NULL,0);
			if (rtn <=0) {
				perror("SeverSend error ");
                /* EAGAIN : Resource temporarily unavailable*/
				if(errno == EINTR || errno == EAGAIN) { 
                    #if 0
					if(SendNum++ > 150) {
						DEBUG_INFO("Resource temporarily unavailable\n");
						break;
					}
					usleep(10*1000);
                    #endif
                    break;
				}
				else {
					DEBUG_INFO("Write error, close socket");
					DeleteSocketRecord(ClientFd);
					break;
				}
				rtn = 0;
			}
			len-=rtn;
		}
		return rtn;
	}
	return 0;
}

void CloseSocketFd(void)
{
	int fds[MAX_CLIENTS], idx = 0;
	GetClientFds(fds, MAX_CLIENTS);
    
	while (GetNumClients() > 1) {
		DeleteSocketRecord(fds[idx++]);
	}
	if (fds[0]) {
		close(fds[0]);
	}
}
