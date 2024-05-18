#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <binary network address>\n",argv[0]);
        exit(0);
    }
    // long int addr1 = atol(argv[1]);
    u_int32_t network_addr = strtoul(argv[1] , NULL, 16);
    // printf("%#x\n",network_addr);
    uint32_t addr = htonl(network_addr);
    char ip_str[INET_ADDRSTRLEN];
    // from network address to point address
    if(inet_ntop(AF_INET,&addr,ip_str,sizeof(ip_str)) == NULL)
    {
        perror("inet_ntop"); 
        exit(-1);
    }
    printf("%s\n",ip_str);
    return 0;
}