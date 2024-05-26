#include "csapp.h"

void transaction(int fd);
void print_request_header(rio_t *rp);
void send_client_msg(int fd,char *errnum,char *shortmsg,char *longmsg,char *cause);
void parse_url(char *url,char *filename,char *cgi_args);
void method_get(int fd,char *url,int head_only);
void method_head(int fd,char *url);

void serve_static(int fd,char *filename,int filesize);
void serve_dynamic(int fd,char *filename,char *cgiargs);
void get_filetype(char *filename,char *filetype);

/* homework 11.8 */
void signal_child_handle(int s)
{
    pid_t pid = waitpid(-1,NULL,0);
    printf("子进程%d结束\n",pid);
    return;
}

int main(int argc,char **argv)
{
    signal(SIGCHLD,signal_child_handle);
    int listenfd,connfd;
    char hostname[MAXLINE],port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <port>\n",argv[0]);
        exit(1);
    }
    // 打开监听文件描述符
    listenfd = Open_listenfd(argv[1]);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        // 接受连接请求
        connfd = Accept(listenfd,(SA*)&clientaddr,&clientlen);
        // 获得连接套接字对应的客户端地址和端口
        getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accept connnection from (%s,%s)\n",hostname,port);
        transaction(connfd);   // 处理事务
        Close(connfd);  // 关闭连接
    }
}

void transaction(int fd)
{
    // 一些字符数组，存放URL和方法等
    char buf[MAXLINE],method[MAXLINE],url[MAXLINE],version[MAXLINE];
    // 初始化缓冲
    rio_t rio;
    Rio_readinitb(&rio,fd);

    // 读一行获取请求头，放入buf
    printf("---->Request<----:\n");
    Rio_readlineb(&rio,buf,MAXLINE);
    printf("%s",buf);
    //  获得方法 URL连接 以及HTTP版本
    sscanf(buf,"%s %s %s",method,url,version);
    //读取其他headers信息
    print_request_header(&rio);

    // 如果method不为GET
    if(strcasecmp(method,"GET") == 0) 
    {
        method_get(fd,url,0);

    }else if(strcasecmp(method,"HEAD") == 0)
    {
        method_head(fd,url);

    }else if(strcasecmp(method,"POST") == 0)
    {

    }else{
        // 给客户端发送错误报文
        send_client_msg(fd,"501","Not implemented","Server doesn't implemented this method",method);
        return;
    }
}

void method_head(int fd,char *url)
{
    method_get(fd,url,1);
    return;
}

void method_get(int fd,char *url,int head_only)
{
    // 获取文件的状态
    struct stat sbuf;
    // 标志：是否为静态请求,默认为动态
    int is_static = 0; 
    // 文件名和cgi动态参数
    char filename[MAXLINE],cgi_args[MAXLINE];
    parse_url(url,filename,cgi_args);// 解析url
    if(!strstr(filename,"cgi-bin")) 
    {
        is_static = 1;  // 静态请求
    }
    // 将filename的文件属性信息保存到sbuf
    if(stat(filename,&sbuf) < 0)    // filename指定路径的文件/目录不存在，返回-1
    {
        // 发生给客户端404错误报文
        send_client_msg(fd,"404","Not found","Server couldn't find the file",filename);
        return;
    }
    if(S_ISREG(sbuf.st_mode))   // 普通文件，而不是目录文件或者套接字文件
    {
        // is_static == 1 表示是静态文件
        if(is_static == 1) 
        {
            // 用户没有读取权限
            if(!(S_IRUSR&sbuf.st_mode))
            {
                send_client_msg(fd,"403","Forbidden","Server couldn't read the file",filename);
                return;
            }
            serve_static(fd,filename,sbuf.st_size);
        }else{
            // 动态文件，但是没有执行权
            if(!(S_IXUSR&sbuf.st_mode))
            {
                send_client_msg(fd,"403","Forbidden","Server couldn't run the file",filename);
                return;
            }
            serve_dynamic(fd,filename,cgi_args);
        }
    }else{
        send_client_msg(fd,"400","Bad Request","This is not a regular file",filename);
    }
    return;
}

/* 读取除开请求行之外的请求报文首部 */
void print_request_header(rio_t *rp)
{
    char buf[MAXLINE];
    Rio_readlineb(rp,buf,MAXLINE);
    // 如果读到空的行\r\n,就退出循环
    // 如果没有读到\r\n，说明请求报头没有结束，继续读下一行
    while(strcmp(buf,"\r\n") == 1)
    {
        Rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);
    }
    return;
}

/* 给客户端返回错误响应报文 */
void send_client_msg(int fd,char *errnum,char *shortmsg,char *longmsg,char *cause)
{
    char line[MAXBUF],header[MAXBUF],body[MAXBUF],tmp[MAXBUF];

    // 生成响应主体 
    sprintf(body,"<html>\r\n<title>Server Error</title>");
    strcat(body,"<body bgcolor=""ffffff"">\r\n");
    strcat(body,"<em>The Tiny webserver</em><hr>\r\n");
    sprintf(tmp,"%s: %s\r\n",errnum,shortmsg);
    strcat(body,tmp);
    sprintf(tmp,"<p>%s: %s</p>\r\n",longmsg,cause);
    strcat(body,tmp);
    strcat(body,"</body>\r\n</html>\r\n");

    // 响应行： HTTP版本 状态码 状态消息
    sprintf(line,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd,line,strlen(line));
    
    // 响应报头： Server: Tiny Web Server
    strcat(header,"Server: Tiny Web Server\r\n");
    // 响应报头：Content-type
    strcat(header,"Content-type: text/html\r\n");
    // 响应报头：Content-length
    sprintf(tmp,"Content-length:%d\r\n",(int)strlen(body));
    strcat(header,tmp);
    // 响应报头：Connection: close
    strcat(header,"Connection: close\r\n");
    // 跟随一个终止报头的空行
    strcat(header,"\r\n");
    Rio_writen(fd,header,strlen(header));
    printf("---->Response<----:\n");
    printf("%s",line);
    printf("%s",header);

    // 发送响应主体
    Rio_writen(fd,body,strlen(body));
    return;
}

/* 从url中分割出文件名和参数名 */
void parse_url(char *url,char *filename,char *cgi_args)
{
    char *ptr = NULL;
    ptr = strchr(url,'?');// 返回？在uri中第一个出现的位置
    // 找到'?'
    if(ptr)
    {
        // 把链接中的参数部分拷贝进去
        strcpy(cgi_args,ptr+1);
        *ptr = '\0';
    }else
        strcpy(cgi_args,"");//如果没有找到？,则没有参数，置空
    strcpy(filename,".");
    strcat(filename,url);
    if(strcmp(filename,"./") == 0)
    {
        strcat(filename,"html/index.html");//拼接默认对主页html文件
    }
    return;
}

/* 服务静态文件 */
void serve_static(int fd,char *filename,int filesize)
{
    int srcfd;
    char *srcp,filetype[MAXLINE],buf[MAXBUF],tmp[MAXBUF];
    get_filetype(filename,filetype);    // 获取文件类型，用于填充响应报头Content-type
    sprintf(buf,"HTTP/1.0 200 OK\r\n"); // 成功响应
    //sprintf(buf,"%sServer: Tiny Web Server\r\n",buf);
    strcat(buf,"Server: Tiny Web Server\r\n");
    //sprintf(buf,"%sConnection: close\r\n",buf);
    strcat(buf,"Connection: close\r\n");
    //sprintf(buf,"%sContent-length: %d\r\n",buf,filesize);
    sprintf(tmp,"Content-length: %d\r\n",filesize);
    strcat(buf,tmp);
    //sprintf(buf,"%sContent-type: %s\r\n\r\n",buf,filetype);
    sprintf(tmp,"Content-type: %s\r\n\r\n",filetype);
    strcat(buf,tmp);    
    Rio_writen(fd,buf,strlen(buf));

    printf("---->Response<----:\n");
    printf("%s",buf);

    // 打开一个文件
    srcfd = Open(filename,O_RDONLY,0);
    // mmap()系统调用使得一个普通文件映射到进程的虚拟区域
    // 普通文件被映射到进程地址空间后
    // 进程可以像访问普通内存一样对文件进行访问
    // NULL 设置为NULL时表示由系统决定映射区的起始地址
    // filesize 以字节为单位不足一内存页按一内存页处理
    // PROT_READ 页内容可以被读取
    // MAP_PRIVATE 建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件
    srcp = Mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
    Close(srcfd);
    // 从srcp开始读
    Rio_writen(fd,srcp,filesize);
    // 在进程地址空间中解除一个映射关系,addr是调用mmap()时返回的地址，len是映射区的大小
    Munmap(srcp,filesize);
    return;
}

/* 服务动态文件 */
void serve_dynamic(int fd,char *filename,char *cgiargs)
{
    char buf[MAXLINE],*emptylist[] = {NULL};
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Server: Tiny Web Server\r\n");
    Rio_writen(fd,buf,strlen(buf));
    // 创建子进程
    if(Fork() == 0)
    {
        //设置环境变量QUERY_STRING的值为cgiargs，1表示可重写，即如果QUERY_STRING已经存在，则覆盖它
        setenv("QUERY_STRING",cgiargs,1);
        //将子进程的标准输出重定向到fd描述符
        //这个的fd为客户端连接套接字文件描述符
        Dup2(fd,STDOUT_FILENO);
        // 指向filename指向的动态文件
        // emptylist是要调用的程序执行的参数序列，也就是我们要调用的程序需要传入的参数
        // environ 同样也是参数序列，一般来说他是一种键值对的形式 key=value. 作为我们是新程序的环境
        Execve(filename,emptylist,environ);
    }
    //父进程等待子进程结束
    // Wait(NULL);
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
            strcpy(filetype,"imgage/JPG");
    else
        strcpy(filetype,"text/plain");
    return;
}