#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

typedef struct {
	void *next;
	int SocketFd;
	socklen_t ClientAddrLen;
	struct sockaddr_in ClientAddr;
	uint8_t IsConnect;
	uint8_t ReadMoreNum ;
	uint8_t ReadMoreData[5];
} SocketRecord_t;

#define MAX_CLIENTS 50
#define LISTEN 20
#define EVENT_SIZE LISTEN
#define EPOLL_SIZE EVENT_SIZE
#define MAXLINE 5

SocketRecord_t *GetAuthBySockfd(int ClinetFd);
int SocketSeverInit(int port);
void GetClientFds(int *fds, int maxFds);
int GetNumClients(void);
void SeverPoll(int ClinetFd, int revent);
int SeverSend(uint8_t* buf, int len, int ClientFd);
void CloseSocketFd(void);
void DeleteSocketRecord(int RmSocketFd);
int CreateSocketRecord(void);
void AddEpollEvent(int SockFd);
void DelEpollEvent(int SockFd);
void SetLinger(const int sock, unsigned l_onoff);
void* SeverEpoll(void* userData);
extern int epfd;

#endif
