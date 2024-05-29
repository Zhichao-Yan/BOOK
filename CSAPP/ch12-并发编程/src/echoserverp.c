/* 
 * echoserverp.c - A concurrent echo server based on processes
 */
#include "csapp.h"
void echo(int connfd);

void sigchld_handler(int sig) 
{
    while (waitpid(-1, 0, WNOHANG) > 0)
	;
    return;
} 

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(argv[1]);
    while (1) 
    {
        clientlen = sizeof(struct sockaddr_storage); 
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        if (Fork() == 0) { 
            Close(listenfd); /* Child closes its listening socket */
            echo(connfd);    /* Child services client */ //line:conc:echoserverp:echofun
            Close(connfd);   /* Child closes connection with client */ //line:conc:echoserverp:childclose
            exit(0);         /* Child exits */
        }
	    Close(connfd); /* Parent closes connected socket (important!) */ //line:conc:echoserverp:parentclose
    }
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio,connfd);
    while((n = Rio_readlineb(&rio,buf,MAXLINE)) != 0) // 从rio读取了n个字节在buf中
    {
        printf("server received %d bytes\n",(int)n); // 显示收到了n个字节
        printf("%s",buf);
        Rio_writen(connfd,buf,n); // 写回buf中n个字节到connfd
    }
}