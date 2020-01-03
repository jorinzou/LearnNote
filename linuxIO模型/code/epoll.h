#ifndef _EPOLL_H_
#define _EPOLL_H_
#include <sys/epoll.h>
#include <stdlib.h>
#define DEBUG_INFO(fmt, args...) printf("\033[33m[%s:%s:%d]\033[0m "#fmt"\r\n", __FILE__,__FUNCTION__, __LINE__, ##args)
#define MAXEVENTS 1000
int  m_epoll_create(int flags);
void m_epoll_add(int fd);
void m_epoll_mod(int fd);
void m_epoll_del(int fd);
int  m_epoll_wait(struct epoll_event *events, int maxevents, int timeout);
struct epoll_event *malloc_epoll_events(int events_num);
extern int epfd;
#endif
