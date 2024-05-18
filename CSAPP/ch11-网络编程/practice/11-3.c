#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <point network address>\n",argv[0]);
        exit(0);
    }
    u_int32_t network_addr;
    if(inet_pton(AF_INET,argv[1],&network_addr) != 1)
    {
        perror("inet_pton"); 
        exit(-1);
    }
    printf("%#x\n",ntohl(network_addr));
    return 0;
}