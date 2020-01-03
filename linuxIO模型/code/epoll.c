#include "epoll.h"
int epfd = -1;
int m_epoll_create(int flags) 
{
    int epfd = epoll_create1(flags);
    if (epfd < 0){
        perror("epoll_create1");
        DEBUG_INFO("epoll_create1");
        return -1;
    }
    return epfd;
}

void m_epoll_add(int fd) 
{
    struct epoll_event event;
    //event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;//edge trigger
    event.events = EPOLLIN|EPOLLERR|EPOLLHUP;//default level trigger
    event.data.fd = fd;
    int rc = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    if (rc < 0){
        perror("epoll_ctl");
        DEBUG_INFO("epoll_ctl");
    }
    return;
}

void m_epoll_mod(int fd)
{
    struct epoll_event event;
    //event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;//edge trigger
    event.events = EPOLLIN|EPOLLERR|EPOLLHUP;//default level trigger
    event.data.fd = fd;
    int rc = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
    if (rc < 0){
        perror("epoll_ctl");
        DEBUG_INFO("epoll_ctl");
    }
    return;
}

void m_epoll_del(int fd) 
{
    struct epoll_event event;
    //event.events = EPOLLIN|EPOLLET|EPOLLERR|EPOLLHUP;//edge trigger
    event.events = EPOLLIN|EPOLLERR|EPOLLHUP;//default level trigger
    event.data.fd = fd;
    int rc = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    if (rc < 0){
        perror("epoll_ctl");
        DEBUG_INFO("epoll_ctl");
    }
    return;
}

int m_epoll_wait(struct epoll_event *events, int maxevents, int timeout)
{
    int n = epoll_wait(epfd, events, maxevents, timeout);
    if (n < 0){
        perror("epoll_wait");
        DEBUG_INFO("epoll_wait");
        return 0;
    }
    return n;
}

struct epoll_event *malloc_epoll_events(int events_num)
{
     struct epoll_event *events = (struct epoll_event *)malloc(sizeof(struct epoll_event)*events_num);
     return events;
}
