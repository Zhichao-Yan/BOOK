#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{
    char ip_str[INET_ADDRSTRLEN];
    u_int32_t addr_host[3] = {0x0,0xffffffff,0x7f000001};
    const char* str[3] = {"205.188.160.121","64.12.149.13","205.188.146.23"};
    for(int i = 0; i < 3; ++i)
    {
        uint32_t addr_net = htonl(addr_host[i]);
        // from net address to point address
        if(inet_ntop(AF_INET,&addr_net,ip_str,sizeof(ip_str)) == NULL)
        {
            perror("inet_ntop"); 
            exit(-1);
        }
        printf("point address of %#x is %s\n",addr_host[i],ip_str);
        // from point address to net address
        if(inet_pton(AF_INET,str[i],&addr_net) != 1)
        {
            perror("inet_pton"); 
            exit(-1);
        }
        printf("net address of %s is %#x \n",str[i],ntohl(addr_net));
    }
}