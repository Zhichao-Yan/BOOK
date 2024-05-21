#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri,char *filename,char *cgiargs);
void clienterror(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg);
void serve_static(int fd,char *filename,int filesize);
void serve_dynamic(int fd,char *filename,char *cgiargs);
void get_filetype(char *filename,char *filetype);


int main(int argc,char **argv)
{
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
        doit(connfd);   // 处理事务
        Close(connfd);  // 关闭连接
        printf("goodbye to (%s,%s)\n",hostname,port);       // say goodbye
    }
}
void doit(int fd)
{
    // 标志：是否为静态请求
    int is_static;
    // 获取文件的状态
    struct stat sbuf;
    char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char filename[MAXLINE],cgiargs[MAXLINE];
    // 初始化缓冲
    rio_t rio;
    Rio_readinitb(&rio,fd);
    // 读一行，获取请求头，放入buf
    Rio_readlineb(&rio,buf,MAXLINE);
    printf("Request headers:\n");
    printf("%s",buf);
    //  获得方法 URL连接 以及HTTP版本
    sscanf(buf,"%s %s %s",method,uri,version);
    // 如果method不为GET
    if(strcasecmp(method,"GET")) 
    {
        // 给客户端发送错误报文
        clienterror(fd,method,"501","Not implemented","Tiny doesn't implemented this method");
        return;
    }
    //读取其他headers信息并且忽略
    read_requesthdrs(&rio);
    is_static = parse_uri(uri,filename,cgiargs);// 解析uri后判断是否为动态请求

    // 将filename的文件属性信息保存到sbuf
    if(stat(filename,&sbuf) < 0)    // 检索文件失败
    {
        // 发生给客户端404错误报文
        clienterror(fd,filename,"404","Not found","Tiny couldn't find this file");
        return;
    }
    // is_static == 1 表示是静态文件
    if(is_static)
    {
        // 文件不是普通文件 或者 它是普通文件但是它不可读
        if(!(S_ISREG(sbuf.st_mode))||!(S_IRUSR&sbuf.st_mode))
        {
            // 发送客户端403错误报文，目标文件找到但是不可读
            clienterror(fd,filename,"403","Forbidden","Tiny couldn't read the file");
            return;
        }
        // 服务静态内容
        serve_static(fd,filename,sbuf.st_size);
    }else{
        // 文件不是普通文件 或者 其是普通文件但是文件不可执行
        if(!(S_ISREG(sbuf.st_mode))||!(S_IXUSR&sbuf.st_mode))
        {
            printf("%s-%s\n",filename,cgiargs);
            clienterror(fd,filename,"403","Forbidden","Tiny couldn't run the CGI file");
            return;
        }
        // 服务动态内容
        serve_dynamic(fd,filename,cgiargs);
    }

}

/* 读取除开请求行之外的请求报头 */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    Rio_readlineb(rp,buf,MAXLINE);
    // 如果读到空的行\r\n,就退出循环
    while(strcmp(buf,"\r\n"))
    {
        Rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);
    }
    return;
}

// 把uri字符串解析成文件名和cgi参数
int parse_uri(char *uri,char *filename,char *cgiargs)
{
    char *ptr;
    // 动态文件保存在cgi-bin目录中，如果没有该子串，说明为静态文件
    if(!strstr(uri,"cgi-bin"))  // uri不包含动态文件，因此为静态
    {
        strcpy(cgiargs,"");     // 给cgiargs赋值为空，因为请求静态文件，不需要参数
        strcpy(filename,".");
        strcat(filename,uri);   // 拼接字符串
        if(uri[strlen(uri)-1] == '/') // 如果相对路径以/结尾
        {
            strcat(filename,"html/home.html");//拼接默认对主页html文件
        }
        return 1;
    }else{
        ptr = strchr(uri,'?');// 返回？在uri中第一个出现的位置
        // 如果找到？，说明动态内容带有参数
        if(ptr)
        {
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }else{
            strcpy(cgiargs,"");//如果没有找到？,则没有参数，置空
        }
        strcpy(filename,".");
        strcat(filename,uri);
        return 0;
    }
}

/* 给客户端返回错误响应报文 */
void clienterror(int fd,char *cause,char *errnum,char *shortmsg,char *longmsg)
{
    char buf[MAXLINE],body[MAXBUF],tmp[MAXBUF];
    // 响应行： HTTP版本 状态码 状态消息
    sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd,buf,strlen(buf));
    // 响应报头：Content-type
    sprintf(buf,"Content-type: text/html\r\n");
    Rio_writen(fd,buf,strlen(buf));

    // 生成响应主体 
    sprintf(body,"<html>\r\n<title>Tiny Error</title>");
    //sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
    strcat(body,"<body bgcolor=""ffffff"">\r\n");
    //sprintf(body,"%s %s:%s\r\n",body,errnum,shortmsg);
    sprintf(tmp," %s:%s\r\n",errnum,shortmsg);
    strcat(body,tmp);
    //sprintf(body,"%s<p>%s:%s</p>\r\n",body,longmsg,cause);
    sprintf(tmp,"<p>%s:%s</p>\r\n",longmsg,cause);
    strcat(body,tmp);
    //sprintf(body,"%s</hr><em>The Tiny webserver</em>\r\n",body);
    strcat(body,"</hr><em>The Tiny webserver</em>\r\n");
    //sprintf(body,"%s</body>\r\n</html>\r\n",body);
    strcat(body,"</body>\r\n</html>\r\n");

    // 响应报头：Content-length
    // 跟随一个终止报头的空行
    sprintf(buf,"Content-length:%d\r\n\r\n",(int)strlen(body));
    Rio_writen(fd,buf,strlen(buf));
    // 发送响应主体
    Rio_writen(fd,body,strlen(body));
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

    printf("Response headers:\n");
    printf("%s",buf);

    srcfd = Open(filename,O_RDONLY,0);
    // 将文件以页为单位映射到内存的虚拟地址空间
    // 0 设置为0时表示由系统决定映射区的起始地址
    // filesize 以字节为单位，不足一内存页按一内存页处理
    // PROT_READ 页内容可以被读取
    // MAP_PRIVATE //建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件
    // srcfd有效的文件描述词
    // mmap()系统调用使得进程之间通过映射同一个普通文件实现共享内存。
    // 普通文件被映射到进程地址空间后，进程可以像访问普通内存一样对文件进行访问，
    srcp = Mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
    Close(srcfd);
    Rio_writen(fd,srcp,filesize);
    // 该调用在进程地址空间中解除一个映射关系,addr是调用mmap()时返回的地址，len是映射区的大小
    Munmap(srcp,filesize);// 避免潜在的内存泄露
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
    Wait(NULL);
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
    else if(strstr(filename,".jpg"))
            strcpy(filetype,"imgage/jpeg");
    else
        strcpy(filetype,"text/plain");
    return;
}