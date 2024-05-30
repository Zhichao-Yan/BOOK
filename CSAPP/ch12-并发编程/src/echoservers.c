#include "csapp.h"

typedef struct 
{ 
    int maxfd;        // 记录最大文件描述符
    fd_set read_set;  // 读集合
    fd_set ready_set; // 已经准备好读的描述符集合
    int nready;       // 已经准备好读的描述符数量，用于check_clients中
    int maxi;         // maxi记录clientfd的最大索引值，这样check就不用搜索整个数组
    int clientfd[FD_SETSIZE];    // 已连接描述符集合
    rio_t clientrio[FD_SETSIZE]; // 已连接描述符对应的缓冲区，用于调用readlinen等带缓冲带IO函数
} pool; 

void init_pool(int listenfd, pool *p);      // 初始化池
void add_client(int connfd, pool *p);       // 把连接描述符加入池
void check_clients(pool *p);

int byte_cnt = 0; /* Counts total bytes received by server */

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    static pool pool; 

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);  // 返回监听套接字
    init_pool(listenfd, &pool); // 初始化池

    while (1) 
    {
        /* 每次Select都会改变池中的ready_set和nready,用于检测事件*/
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);
    
        if (FD_ISSET(listenfd, &pool.ready_set))    // 如果监听描述符准备好了
        { 
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);   // 接受连接描述符
            add_client(connfd, &pool);  // 将客户端添加到池中
        }
	    check_clients(&pool); 
    }
}

/* 初始化池 */
void init_pool(int listenfd, pool *p) 
{
    int i;
    p->maxi = -1;                   // 一开始整个clientfd为空，所以最大索引为-1
    for (i=0; i< FD_SETSIZE; i++)   // 初始时，已连接描述符集合为空
	    p->clientfd[i] = -1;        // -1表示一个已连接描述符集合的可用的槽位

    p->maxfd = listenfd;            // 一开始最大的描述符是监听套接字
    FD_ZERO(&p->read_set);          // 置空读集合
    FD_SET(listenfd, &p->read_set); // 把监听描述符加入读集合
}

/* 添加连接描述符到池 */
void add_client(int connfd, pool *p) 
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++)  
    {
        if (p->clientfd[i] < 0)     // 找到一个空的槽位
        { 
            p->clientfd[i] = connfd;               // 添加到槽位
            Rio_readinitb(&p->clientrio[i], connfd); // 初始化对应的缓冲区，方便后续读行
        
            FD_SET(connfd, &p->read_set); // 将该连接描述符加入到读集合

            if (connfd > p->maxfd)  // 更新最大的文件描述符
                p->maxfd = connfd; 
            if (i > p->maxi)       // 更新clientfd最大索引值
                p->maxi = i;       
            break;
        }
    }    
    if (i == FD_SETSIZE) // 无法找到空的槽位
	    app_error("add_client error: Too many clients");
}

/* 检测客户端是否有需要读的 */
void check_clients(pool *p) 
{
    int i, connfd, n;
    char buf[MAXLINE]; 
    rio_t rio;

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) 
    {
        connfd = p->clientfd[i];
        rio = p->clientrio[i];
        // 如果槽位非空，该连接描述符在已经准备好读的集合中
        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) 
        { 
            p->nready--;    // 准备好读--
            if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) // 从中读取一行
            {
                byte_cnt += n;  // 累计读取的字节数目
                printf("Server received %d (%d total) bytes on fd\
                 %d\n", n, byte_cnt, connfd);   // 打印读取结果
                Rio_writen(connfd, buf, n);     // 回收读取行
            }
            else    // 客户端关闭连接的它那一段，服务器端遇到EOF
            { 
                Close(connfd);                  // 关闭连接描述符
                FD_CLR(connfd, &p->read_set);   // 从读集合中清楚该描述符
                p->clientfd[i] = -1;            // clientfd槽位置空
            }
        }
    }
}

