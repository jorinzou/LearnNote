#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>

/*
获取网卡信息
1.if_name 网卡名称
2.addr_type SIOCGIFBRDADDR(广播地址) ,SIOCGIFADDR(ip地址)
3.addr 出参,可以为NULL
*/
const char* get_ipaddr(const char *if_name,unsigned short addr_type,struct sockaddr_in *addr)
{
    int sock;
    int ret;
    struct ifreq ifr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        return NULL;
    }
    strcpy(ifr.ifr_name, if_name);/*网卡名称*/
    ret = ioctl(sock, addr_type, &ifr);
    if (ret < 0){
        return NULL;
    }
    if (addr != NULL){
        memcpy(addr,&(ifr.ifr_addr),sizeof(*addr));
    }
    return inet_ntoa(((struct sockaddr_in *)(&(ifr.ifr_addr)))->sin_addr);
}


int main(void)
{
	const char *ip = get_ipaddr("ens33",SIOCGIFADDR,NULL);
	printf("%s\n",ip);
	return 0;
}

