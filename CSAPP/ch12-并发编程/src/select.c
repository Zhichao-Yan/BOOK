/* $begin select */
#include "csapp.h"
void echo(int connfd);
void command(void);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set; // 定义读集合和准备好集合的变量
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);  // 打开一个监听描述符

    FD_ZERO(&read_set);              // 清空读集合
    FD_SET(STDIN_FILENO, &read_set); // 把标准输入添加到读集合
    FD_SET(listenfd, &read_set);     // 把监听描述符也添加到读集合

    while (1)   // 循环
    {
        ready_set = read_set;   // 把读集合赋值给准备好集合
        /* 调用Select，直到监听描述符或者标准输入可读
         * Select返回后，会设置ready_set为实际准备好的描述符集合，如只包含标准输入
         */
        Select(listenfd+1, &ready_set, NULL, NULL, NULL); 
        if (FD_ISSET(STDIN_FILENO, &ready_set)) // 如果是标准输入准备好了，说明可以执行命令了
            command(); 
        if (FD_ISSET(listenfd, &ready_set)) // 或者是监听描述符准备好了，说明可以接受连接了
        { 
            clientlen = sizeof(struct sockaddr_storage); 
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            echo(connfd); 
            Close(connfd);
        }
    }
}

void command(void) 
{
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
	    exit(0); /* EOF */
    printf("%s", buf); 
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
