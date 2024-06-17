#include "csapp.h"
#include "sbuf.h"
#include "queue.h"
#include "http.h"

#define SBUFSIZE 16
#define QUEUE_SIZE 100
#define MINI_THREADS 4
#define HEADER_LIST 10
#define HEADER_LIST_INCREMENT 10


#define localhost "172.16.153.130"
#define localport "4000"

char* find_header(header_list *L,const char *name);
void init_header_list(header_list *ptr);
void deinit_header_list(header_list *ptr);
void read_header_list(rio_t *rp,header_list *L);
void append_header_list(header_list *L,const char *name,const char *value);
void generate_header(header_list *L,char *header);

void send_response(int fd,const char *line,const char *header,const char *body);
void send_error_response(int fd,char *version,const char *code,const char *status,const char *msg,const char *cause,int head_only);
void transaction(int fd);
void parse_url(char *url,char *scheme,char *host,char *path);
void parse_path(char *path,char *filename,char *cgi_args);
void parse_host(char *host,char *port);

void method_get(int fd,char *filename,char *args,int head_only);
void method_head(int fd,char *filename,char *args);
void method_post(int fd,char *filename,char *body);
void get_filetype(char *filename,char *filetype);
void serve_static(int fd,char *filename,char *args,int head_only);
void serve_dynamic(int fd,char *filename,char *cgi_args,int head_only);

void signal_child_handle(int sig);
void ignore_sigpipe(int sig);
void* worker(void *arg);
void* manager(void *arg);

int connect_to_server(char *host,char *port);
void forward_data(int clientfd, int serverfd);
void proxy_service(int clientfd,char *host,char *port,char *method,char *url,char *version);


sbuf_t sbuf; // 缓冲
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // 条件变量


void send_response(int fd,const char *line,const char *header,const char *body)
{
    char buf[MAXBUF] = {'\0'};
    if(line != NULL)
    {
        strcpy(buf,line);
    }
    if(header != NULL)
    {
        strcat(buf,header);
    }
    if(body != NULL)
    {
        strcat(buf,body);
    }
    rio_writen(fd,buf,strlen(buf));
    return;
}


/* 给客户端返回错误响应报文 */
void send_error_response(int fd,char *version,const char *code,const char *status,const char *msg,const char *cause,int head_only)
{
    char tmp[MAXBUF] = {'\0'};  // 临时字符数组
    // 错误报文实体主体
    char body[MAXBUF] = {'\0'};
    strcpy(body, "<html>\r\n");           // 开始HTML文档
    strcat(body, "<head><title>Server Error</title></head>\r\n");
    strcat(body, "<body>\r\n");           // 开始body部分
    strcat(body, "<em>The Tiny webserver</em>\r\n"); // 添加标题
    strcat(body, "<hr>\r\n");    // 水平线
    sprintf(tmp, "%s: %s\r\n", code, status); // 格式化错误代码和状态
    strcat(body, tmp);
    sprintf(tmp, "<p>%s: %s</p>\r\n", msg, cause); // 格式化错误消息和原因
    strcat(body, tmp);
    strcat(body, "</body>\r\n</html>\r\n"); // 结束body和HTML文档

    // 响应报头： Server: Tiny Web Server
    char header[MAXBUF] = {'\0'};
    strcpy(header,"Server: Tiny Web Server\r\n");
    // 响应报头：Content-type
    strcat(header,"Content-type: text/html\r\n");
    // 响应报头：Content-length
    strcpy(tmp,"");
    sprintf(tmp,"Content-length:%d\r\n",(int)strlen(body));
    strcat(header,tmp);
    // 响应报头：Connection: close
    strcat(header,"Connection: close\r\n");
    // 跟随一个终止报头的空行
    strcat(header,"\r\n");

    // 响应行： HTTP版本 状态码 状态消息
    char line[MAXBUF] = {'\0'};
    sprintf(line,"%s %s %s\r\n",version,code,status);

    printf("---->Response<----:\n");
    printf("%s",line);
    printf("%s",header);

    if(head_only == 1)
    {
        send_response(fd,line,header,NULL);
    }else{
        send_response(fd,line,header,body); 
    }
    return;
}


/* 工作线程 */
void* worker(void *arg)
{
    Pthread_detach(Pthread_self());
    while(1)
    {
        int oldstate;
        /* 
        *    取消状态再次变成PTHREAD_CANCEL_DISABLE
        *    取消请求会被挂起，这意味这在下面的网络通信中
        *    线程不会被取消，直到取消状态再次变成PTHREAD_CANCEL_ENABLE
        *  */
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);   // 线程暂时不可取消   
        int connfd = sbuf_remove(&sbuf);
        transaction(connfd);   // 处理事务
        // Close(connfd);  // 并不是所有都需要关闭连接，我们可以使用持久连接
        pthread_setcancelstate(oldstate,NULL);  // 取消状态再次变成PTHREAD_CANCEL_ENABLE，可以接受取消
        pthread_testcancel();   // 设置取消点，线程在此检查是否有取消请求，如果有，就会pthread_exit()
    }
}

/* 管理者线程例程 */
void* manager(void *arg)
{
    /* 初始化线程id队列 */
    queue q;
    init_queue(&q,QUEUE_SIZE);

    pthread_t tid;
    /* 初始创建MINI_THREADS个线程*/
    for(int i = 0; i < MINI_THREADS; ++i)
    {
        Pthread_create(&tid,NULL,worker,NULL);
        queue_push(&q,tid);
    }
    unsigned long cnt = MINI_THREADS;   // cnt记录当前的线程数量
    while(1)
    {
        pthread_mutex_lock(&(sbuf.mutex));
        /* 
        *   已知当缓冲区为空或者为满的时候，sbuf.front == sbuf.rear
        *   如果不满足该条件，则释放互斥量，并且继续睡眠等待。
        *   当收到满或者空的信号时醒过来，重新获得锁，并且检查条件
        *   如果不满足循环，则退出循环
        *  */
        while(sbuf.front != sbuf.rear)  
        {
            pthread_cond_wait(&cond,&(sbuf.mutex));
        }
        if(sbuf.count == 0)
        {
            // 如果cnt比规定最小的线程数量还大，则继续减小线程数量，否则即算了
            if(cnt > MINI_THREADS)
            {
                unsigned long i = cnt * 0.5;
                while(i > 0)
                {
                    tid = queue_pop(&q);    // 弹出线程tid
                    --i;            
                    pthread_cancel(tid);    // 取消线程tid
                }
                cnt = cnt * 0.5;
            }
        }else if(sbuf.count == sbuf.n)
        {
            // double线程数量
            for(unsigned long i = 0; i < cnt; ++i)
            {
                Pthread_create(&tid,NULL,worker,NULL);
                queue_push(&q,tid);
            }
            cnt = 2 * cnt;
        }
        pthread_mutex_unlock(&(sbuf.mutex));
    }
}

int main(int argc,char **argv)
{
    signal(SIGCHLD,signal_child_handle);
    signal(SIGPIPE, ignore_sigpipe);
    int listenfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; // 用于存储客户端套接字地址
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <port>\n",argv[0]);
        exit(1);
    }
    // 打开监听文件描述符
    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf,SBUFSIZE);  // 初始化缓冲
    /* 创建管理者线程 */
    pthread_t tid;
    Pthread_create(&tid,NULL,manager,NULL);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        int connfd = Accept(listenfd,(SA*)&clientaddr,&clientlen); // 接受连接请求
        // transaction(connfd);   // 处理事务
        sbuf_insert(&sbuf,connfd);
    }
}

/* 从缓冲区读取首部信息并合成首部表 */
void read_header_list(rio_t *rp,header_list *L)
{
    char buf[MAXLINE];
    Rio_readlineb(rp,buf,MAXLINE); // 事先读取一行
    while(strcmp(buf,"\r\n") != 0)  // 如果不等于回车换行符
    {
        /* 查找首部名 */
        char *ptr = strchr(buf,':');
        *ptr = '\0';
        char *name = buf;

        /* 查找首部名对应的值 */
        ptr = ptr + 2;
        char *end = strpbrk(ptr,"\r\n");  // 找到第一个属于"\r\n"的字符的位置
        *end = '\0';
        char *value = ptr;

        append_header_list(L,name,value);   // 将name和value添加到L尾部

        Rio_readlineb(rp,buf,MAXLINE);  // 读取下一行
    }
    return;
}

/* 添加头部到header_list中 */
void append_header_list(header_list *L,const char *name,const char *value)
{
    if(L->count == L->size)  // 如果表已经满了,扩展表
    {
        header *ptr = (header*)realloc(L->list,(L->size + HEADER_LIST_INCREMENT) * sizeof(header));
        if(!ptr)    // failed
        {
            perror("realloc failed\n");
            exit(-1);
        }else{
            L->list = ptr;
            L->size += HEADER_LIST_INCREMENT;
        }
    }
    unsigned index = L->count;
    strcpy(L->list[index].name,name);
    strcpy(L->list[index].value,value);
    L->count++;
    return;
}

/* 初始化首部表 */
void init_header_list(header_list *ptr)
{
    ptr->list = (header*)malloc(HEADER_LIST * sizeof(header));
    if(!ptr->list)
    {
        perror("malloc error\n");
        exit(-1);
    }
    ptr->size = HEADER_LIST;
    ptr->count = 0;
    return;
}

/* 释放首部表 */
void deinit_header_list(header_list *ptr)
{
    free(ptr->list);
}

/* 从首部表中查找name对应的value */
char* find_header(header_list *L,const char *name)
{
    char *ptr = NULL;
    for(int i = 0; i < L->count; ++i)
    {
        if(strcmp(L->list[i].name,name) == 0)
        {
            ptr = L->list[i].value;
            break;
        }
    }
    return ptr;
}

/* 由header_list拼接成header字符串 */
void generate_header(header_list *L,char *header)
{
    char tmp[MAXLINE];
    int count = L->count;
    for(int i = 0; i < count; ++i)
    {

        sprintf(tmp,"%s: %s\r\n",L->list[i].name,L->list[i].value);
        strcat(header,tmp);
    }
    return;
}

/* 从url字符串中提取协议方案/主机名端口/路径 */
void parse_url(char *url,char *scheme,char *host,char *path)
{
    char *start = url,*ptr = NULL;
    ptr = strstr(start,"://");
    if(!ptr)    // url中没有scheme
    {
        strcpy(scheme,"");  // 返回scheme空字符串
    }else{
        *ptr = '\0';
        strcpy(scheme,start);
        start = ptr + 3; 
    }

    ptr = strchr(start,'/');
    if(!ptr)    // 如果没有路径
    {
        strcpy(path,"");  // 返回空串
    }else{
        strcpy(path,ptr); // 从/处开始复制，直到结束
        *ptr = '\0';      // 变成结尾符号，方便后面复制host
    }
    strcpy(host,start);
    return;
}


/* 尝试从主机中分割出端口号 */
void parse_host(char *host,char *port)
{
    char *ptr;
    ptr = strchr(host,':');
    if(ptr)
    {
        strcpy(port,ptr + 1);
        *ptr = '\0';
    }else{
        strcpy(port,"");
    }
    return;
}

void transaction(int fd)
{
    /* 初始化缓冲 */
    rio_t rio;
    Rio_readinitb(&rio,fd);

    /* 读取请求行，并获得方法 URL连接 以及HTTP版本 */
    char line[MAXLINE],method[MAXLINE],url[MAXLINE],version[MAXLINE];
    Rio_readlineb(&rio,line,MAXLINE);
    sscanf(line,"%s %s %s",method,url,version);   

    /* 读取首部 */
    header_list L;
    init_header_list(&L);
    read_header_list(&rio,&L);
    
    char header[MAXBUF];
    strcpy(header,"");
    generate_header(&L,header);

    printf("---->Request<----:\n");
    printf("%s",line);
    printf("%s",header);

    
    char scheme[MAXLINE],host[MAXLINE],port[MAXLINE],path[MAXLINE];
    parse_url(url,scheme,host,path);      // 从url中抽取方案、主机端口、路径 
    parse_host(host,port);                       // 尝试从中分割出端口号


    /* 虽然前面获取了url，但是其不一定包含域名和端口号，但是还是以首部为主，因此查找首部，尝试覆盖 */
    char tmp[MAXLINE];
    char *val = find_header(&L,"Host");
    strcpy(tmp,val);
    char *ptr = strchr(tmp,':');
    if(ptr)
    {
        strcpy(port,ptr+1);
        *ptr = '\0';
        strcpy(host,tmp);
    }else{
        strcpy(host,tmp);
    }

    if(strcmp(host,localhost) == 0 && strcmp(port,localport) == 0)
    {
        char filename[MAXLINE],args[MAXLINE];
        parse_path(path,filename,args);     // 从路径中解析得到文件名和参数
        /* 访问本地主机服务 */
        if(strcasecmp(method,"GET") == 0)
        {
            method_get(fd,filename,args,0);
        }else if(strcasecmp(method,"HEAD") == 0)
        {
            method_head(fd,filename,args);
        }else if(strcasecmp(method,"POST") == 0)
        {
            /* 获取实体主体长度 */
            int content_length = 0;
            char *ptr = find_header(&L,"Content-Length");
            content_length = atoi(ptr);
            /* 获取实体主体 */
            char body[MAXLINE];
            Rio_readlineb(&rio,body,content_length+1);
            /* 执行post方法 */            
            method_post(fd,filename,body);
        }else{
            send_error_response(fd,"HTTP/1.1","501","Not implemented","Server doesn't implemented this method",method,0);
        }
        close(fd);
    }else{
        /* 进行代理转发 */
        proxy_service(fd,host,port,method,url,version);
    }
    deinit_header_list(&L);
}

/* 连接目的服务器 */
int connect_to_server(char *host,char *port)
{
    struct addrinfo *result,*p;
    struct addrinfo hints;
    int clientfd;
    memset(&hints, 0, sizeof(struct addrinfo)); //  只能设置某些字段，只好先清空hints
    /* hints提供对getaddrinfo返回的listp指向的套接字链表更好的控制 */
    hints.ai_family  = AF_INET;
    hints.ai_socktype = SOCK_STREAM;      // 对每个地址，规定只返回SOCK_STREAM套接字
    /*  hints.ai_flags 是掩码 */
    hints.ai_flags = AI_ADDRCONFIG;
    int rc;
    if ((rc = getaddrinfo(host, port, &hints, &result)) != 0) 
    {
        fprintf(stderr, "getaddrinfo failed (host-%s port-%s): %s\n", host,port, gai_strerror(rc));
        return -2;
    }
    /* 遍历列表，找到套接字，直到成功 */
    for (p = result; p; p = p->ai_next) 
    {
        /* 直接用返回addrinfo中的数据结构作为参数调用socket函数 */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue;  // 失败了，继续链表中下一个套接字地址

        if(connect(clientfd,p->ai_addr,p->ai_addrlen) == 0)
        {
            break;
        }
        if (close(clientfd) < 0) 
        {   
            fprintf(stderr, "clientfd close failed: %s\n", strerror(errno));
            freeaddrinfo(result);
            return -1;
        }  
    }
    freeaddrinfo(result);
    if (!p)     // p为空，意味着链表检查完，没有套接字地址绑定描述符成功
        return -1;
    return clientfd;
}


/* 转发数据 */
void forward_data(int clientfd, int serverfd)
{

    return;
}

int negotiate(int clientfd, int serverfd)
{
    char buf[MAXBUF] = {'\0'};
    rio_t rio_client,rio_server;
    Rio_readinitb(&rio_client,clientfd);
    Rio_readinitb(&rio_server,serverfd);
    if(fcntl(clientfd,F_SETFL,O_NONBLOCK) < 0)
    {
        perror("Failed setting clientfd status flag\n");
        return -1;
    }
    if(fcntl(serverfd,F_SETFL,O_NONBLOCK) < 0)
    {
        perror("Failed setting servertfd status flag\n");
        return -1;
    }
    int max_fd = (clientfd > serverfd ? clientfd : serverfd); 
    fd_set read_set,ready_set;
    FD_ZERO(&read_set);
    FD_SET(clientfd,&read_set);
    FD_SET(serverfd,&read_set);
    struct timeval timeout;
    timeout.tv_sec = 60; // 等待时间60秒
    timeout.tv_usec = 0; // 微秒，0表示不使用微秒级超时
    int rr,rw;
    while(1)
    {
        ready_set = read_set;
        int activity = select(max_fd + 1, &ready_set, NULL, NULL, &timeout);
        // 检查 select 返回值
        if (activity == 0) 
        {
            printf("select timeout\n");
            return -1; // 超时,退出循环
        } else if (activity > 0)
        {
            if (FD_ISSET(clientfd, &ready_set)) // 客户端可以读了
            {
                rr = rio_readnb(&rio_client,buf,MAXLINE);   // 从客户端读
                if( rr < 0) // 客户端可以读取，但是仍然可能出错或被关闭
                {
                    perror("Reading error from client\n");
                    return -1;
                }else if(rr > 0)
                {
                    rw = rio_writen(serverfd, buf, rr);
                    if( rw < 0) // 转发给服务端
                    {
                        return -1;
                    }
                }else if(rr == 0)
                {
                    printf("Reading EOF from client\n");
                    return -1;
                }
            }else if (FD_ISSET(serverfd, &ready_set)) // 服务端可以读了
            {
                rr = rio_readnb(&rio_server,buf,MAXLINE); // 尝试从服务端读取
                if( rr < 0) // 服务端可以读取，但是仍然可能出错或被关闭
                {
                    perror("Reading error from server\n");
                    return -1;
                }else if(rr > 0)
                {
                    rw = rio_writen(clientfd, buf, rr);
                    if( rw < 0)// 转发给客户端
                    {
                        return -1;
                    }
                }else if(rr == 0)
                {
                    printf("Reading EOF from server\n");
                    return -1;
                }
            }
        }
    }
    return 1;
}

/* 代理服务 */
void proxy_service(int clientfd,char *host,char *port,char *method,char *url,char *version)
{ 
    if (strcasecmp(method, "CONNECT") == 0)  
    {
        // 连接目标服务器
        int serverfd = connect_to_server(host,port);
        if(serverfd < 0)
        {
            send_error_response(clientfd,"HTTP/1.1","503","Service Unavailable","Failed connecting to the Server",host,0);
            close(clientfd);
            return;
        }else{
            char buf[MAXLINE];
            strcpy(buf,"HTTP/1.1 200 Connection Established\r\n");
            strcat(buf,"Server: The Tiny webserver\r\n");
            strcat(buf,"Proxy-Connection: keep-alive\r\n");
            strcat(buf,"Connection: keep-alive\r\n");
            strcat(buf,"\r\n");
            if(rio_writen(clientfd,buf,sizeof(buf)) < 0)
            {
                close(clientfd);
                close(serverfd);
                return;
            }
            if(negotiate(clientfd, serverfd) < 0) // 进行协商，协商成果，则转发数据
            {
                close(clientfd);
                close(serverfd);
                return;
            }
        }
    }
    return;
}


/* homework-11.12 POST方法  */
void method_post(int fd,char *filename,char *body)
{
    serve_dynamic(fd,filename,body,0);
    return;
}

/* head方法（homework 11.11)*/
void method_head(int fd,char *filename,char *args)
{
    method_get(fd,filename,args,1);
    return;
}

/* get方法 */
void method_get(int fd,char *filename,char *args,int head_only)
{
    struct stat sbuf;   // 获取文件的状态
    /* 有没有参数不能决定是否执行动态请求，静态请求也可以有参数，关键在于文件是否是可执行 */
    int flag = 0; // 标志：是否为静态请求。默认情况为flag为0，表示静态请求，
    // 将filename的文件属性信息保存到sbuf
    if(stat(filename,&sbuf) < 0)    // filename指定路径的文件/目录不存在，返回-1
    {
        // 发生给客户端404错误报文
        send_error_response(fd,"HTTP/1.1","404","Not found","Server couldn't find the file",filename,head_only);
        return;
    }
    if(strstr(filename,"cgi-bin"))  // 动态文件夹，转变为动态请求
    {
        flag = 1;  // 动态请求
    }
    if(S_ISREG(sbuf.st_mode))   // 普通文件，而不是目录文件或者套接字文件
    {
        if(flag == 0) 
        {
            // 静态文件，但是用户没有读取权限
            if(!(S_IRUSR&sbuf.st_mode))
            {
                send_error_response(fd,"HTTP/1.1","403","Forbidden","Server couldn't read the file",filename,head_only);
                return;
            }
            serve_static(fd,filename,args,head_only);
        }else{
            // 动态文件，但是没有执行权
            if(!(S_IXUSR&sbuf.st_mode))
            {
                send_error_response(fd,"HTTP/1.1","403","Forbidden","Server couldn't run the file",filename,head_only);
                return;
            }
            serve_dynamic(fd,filename,args,head_only);
        }
    }else{
            send_error_response(fd,"HTTP/1.1","400","Bad Request","This is not a regular file",filename,head_only);
    }
    return;
}

/* 从path中分割出文件名和参数名 */
void parse_path(char *path,char *filename,char *args)
{
    char *ptr = strchr(path,'?');// 返回？在path中第一个出现的位置
    // 找到'?'
    if(ptr)
    {
        // 把链接中的参数部分拷贝进去
        strcpy(args,ptr+1);
        *ptr = '\0';
    }else
        strcpy(args,"");//如果没有找到？,则没有参数，置空
    strcpy(filename,".");
    strcat(filename,path);
    if(strcmp(filename,"./") == 0)
    {
        strcat(filename,"html/index.html");//拼接默认对主页html文件
    }
    return;
}
/* 发送静态文件（mmap映射） */
void serve_static(int fd,char *filename,char *args,int head_only)
{
    int srcfd = Open(filename,O_RDONLY,0);  // 打开一个文件filename并返回文件描述符
    struct stat sbuf;
    if (fstat(srcfd, &sbuf) < 0) 
    {
        perror("文件状态获取失败\n");
        return;
    }
    size_t filesize = sbuf.st_size;
    /* 映射到body */
    char *body = Mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);

    /* 构造响应报文首部*/
    char header[MAXBUF],tmp[MAXLINE],filetype[MAXLINE];
    strcpy(header,"Server: Tiny Web Server\r\n");
    // 响应报头Content-length
    sprintf(tmp,"Content-length: %ld\r\n",filesize);
    strcat(header,tmp);
    // 获取文件类型，用于填充响应报头Content-type
    get_filetype(filename,filetype);    
    sprintf(tmp,"Content-type: %s\r\n",filetype);
    strcat(header,tmp);  
    // 响应报头Connection
    strcat(header,"Connection: close\r\n");
    // 响应报头结尾的回车换行符
    strcat(header,"\r\n");

    // 响应行： HTTP版本 状态码 状态消息
    char line[MAXBUF] = {'\0'};
    strcpy(line,"HTTP/1.1 200 OK\r\n");

    printf("---->Response<----:\n");
    printf("%s",line);
    printf("%s",header);
    send_response(fd,line,header,NULL);

    if(head_only == 0)
    {
        Rio_writen(fd,body,filesize);   // 从body开始写入filesize个字节到fd
    }
    Close(srcfd);   // 关闭文件描述符
    Munmap(body,filesize);      // 在进程地址空间中解除一个映射关系
    return;
}

/* 服务静态文件（malloc方式） homework 11.9 */
void serve_static_malloc(int fd,char *filename,char *args,int head_only)
{
    /* 打开文件并获取文件描述符 */
    int srcfd = Open(filename, O_RDONLY, 0);
    if (srcfd < 0) 
    {
        // 错误处理
        perror("文件打开失败\n");
        return;
    }
    /* 获取文件大小 */
    struct stat sbuf;
    if (fstat(srcfd, &sbuf) < 0) 
    {
        // 错误处理
        perror("无法获取文件状态\n");
        Close(srcfd);
        return;
    }
    size_t filesize = sbuf.st_size;

    char *buf = (char *)malloc(filesize); 
    if (buf == NULL) 
    {
        // 错误处理
        perror("内存分配失败\n");
        Close(srcfd);
        return;
    }
    // 读取文件内容
    ssize_t n = Rio_readn(srcfd, buf, filesize);
    if (n < 0) 
    {
        // 错误处理
        free(buf);
        Close(srcfd);
        return;
    }

    /* 构造响应报文首部*/
    char header[MAXBUF],tmp[MAXLINE],filetype[MAXLINE];
    strcpy(header,"Server: Tiny Web Server\r\n");
    // 响应报头Content-length
    sprintf(tmp,"Content-length: %ld\r\n",filesize);
    strcat(header,tmp);
    // 获取文件类型，用于填充响应报头Content-type
    get_filetype(filename,filetype);    
    sprintf(tmp,"Content-type: %s\r\n",filetype);
    strcat(header,tmp);  
    // 响应报头Connection
    strcat(header,"Connection: close\r\n");
    // 响应报头结尾的回车换行符
    strcat(header,"\r\n");

    // 响应行： HTTP版本 状态码 状态消息
    char line[MAXBUF] = {'\0'};
    strcpy(line,"HTTP/1.1 200 OK\r\n");

    
    printf("---->Response<----:\n");
    printf("%s",line);
    printf("%s",header);
    send_response(fd,line,header,NULL);
    if(head_only == 0)
    {
        Rio_writen(fd,buf,filesize);    // 从body开始写入filesize个字节到fd
    }

    Close(srcfd);   // 关闭文件描述符
    free(buf);  // 释放内存
    return;
}

/* 解析查询参数，放入result中 */
void parse_query_string(char* str,char** result) 
{
    int i = 0;
    char *ptr = NULL;
    while((ptr = strchr(str,'&')))
    {
        result[i++] = str;
        *ptr = '\0';
        str = ptr + 1;
    }
    result[i++] = str;
    result[i] = NULL;
}

/* 服务动态文件 */
void serve_dynamic(int fd,char *filename,char *cgi_args,int head_only)
{
    // 创建子进程
    if(Fork() == 0)
    {
        char head[20];
        sprintf(head,"&head_only=%d",head_only);
        strcat(cgi_args,head);
        // 子进程参数列表
        char *envp[MAXLINE];
        parse_query_string(cgi_args,envp);
        char *argv[] = {filename,NULL};
        //将子进程的标准输出重定向到fd描述符
        //这个的fd为客户端连接套接字文件描述符
        Dup2(fd,STDOUT_FILENO);
        // 指向filename指向的动态文件
        // argv 参数字符串指针数组
        // envp 环境变量字符串指针数组
        Execve(filename,argv,envp);
    }
}

/* 根据文件名，获取文件类型 */
void get_filetype(char *filename,char *filetype)
{
    if(strstr(filename,".html"))
        strcpy(filetype,"text/html");
    else if(strstr(filename,".gif"))
            strcpy(filetype,"imgage/gif");
    else if(strstr(filename,".png"))
            strcpy(filetype,"imgage/png");
    else if(strstr(filename,".jpeg"))
            strcpy(filetype,"imgage/jpeg");
    else if(strstr(filename,".jpg"))
            strcpy(filetype,"imgage/jpg");
    else if(strstr(filename,".JPG"))
            strcpy(filetype,"image/JPG");
    else
        strcpy(filetype,"text/plain");
    return;
}

/* homework 11.8 */
void signal_child_handle(int sig)
{
    pid_t pid = waitpid(-1,NULL,0);
    printf("---->Response<----:\n");
    printf("子进程%d结束\n\n",pid);
    return;
}
/* homework 11.13 */
void ignore_sigpipe(int sig) 
{
    printf("连接被客户端提前关闭\n");
    return;
}




// if(ptr) // tmp存在端口号,把之前获得的主机和端口号进行覆盖，以报文中Host首部为优先
// {
//     strcpy(port,ptr+1);
//     *ptr = '\0';
//     strcpy(host,tmp);
// }else{                  // tmp没有端口号，把主机进行覆盖，如果端口号还是为空，
//     strcpy(host,tmp);   // 覆盖主机
//     if(strcmp(port,"") == 0)    // 如果目前端口号仍然为空，即之前没有获取到
//     {
//         if(strcmp(scheme,"") == 0)  // 并且方案也为空
//         {
//             strcpy(port,"443"); // 赋予默认的443端口号
//         }else if(strcasecmp(scheme,"HTTP") == 0)    // 如果协议方案为http
//         {
//             strcpy(port,"80");  // 赋予80端口号
//         }else if(strcasecmp(scheme,"HTTPS") == 0)   // 如果协议方案为https
//         {
//             strcpy(port,"443"); // 赋予443端口号
//         }
//     }
//     // 否则依照之前获取的端口号
// }

    // close(clientfd);
    // close(serverfd);