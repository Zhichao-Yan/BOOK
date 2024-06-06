/* 
 * csapp.c - Functions for the CS:APP3e book
 *
 * Updated 10/2016 reb:
 *   - Fixed bug in sio_ltoa that didn't cover negative numbers
 *
 * Updated 2/2016 droh:
 *   - Updated open_clientfd and open_listenfd to fail more gracefully
 *
 * Updated 8/2014 droh: 
 *   - New versions of open_clientfd and open_listenfd are reentrant and
 *     protocol independent.
 *
 *   - Added protocol-independent inet_ntop and inet_pton functions. The
 *     inet_ntoa and inet_aton functions are obsolete.
 *
 * Updated 7/2014 droh:
 *   - Aded reentrant sio (signal-safe I/O) routines
 * 
 * Updated 4/2013 droh: 
 *   - rio_readlineb: fixed edge case bug
 *   - rio_readnb: removed redundant EINTR check
 */
/* $begin csapp.c */
#include "csapp.h"
/************************** 
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */


void posix_error(int code, char *msg) /* Posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void gai_error(int code, char *msg) /* Getaddrinfo-style error */
{
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
    exit(0);
}

void app_error(char *msg) /* Application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

void dns_error(char *msg) /* Obsolete gethostbyname error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}


/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

/* $begin forkwrapper */
pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}
/* $end forkwrapper */

void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
    if (execve(filename, argv, envp) < 0)
	unix_error("Execve error");
}

/* $begin wait */
pid_t Wait(int *status) 
{
    pid_t pid;

    if ((pid  = wait(status)) < 0)
	unix_error("Wait error");
    return pid;
}
/* $end wait */

pid_t Waitpid(pid_t pid, int *iptr, int options) 
{
    pid_t retpid;

    if ((retpid  = waitpid(pid, iptr, options)) < 0) 
	unix_error("Waitpid error");
    return(retpid);
}

/* $begin kill */
void Kill(pid_t pid, int signum) 
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
	unix_error("Kill error");
}
/* $end kill */

void Pause() 
{
    (void)pause();
    return;
}

unsigned int Sleep(unsigned int secs) 
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
	unix_error("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}
 
void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
	unix_error("Setpgid error");
    return;
}

pid_t Getpgrp(void) {
    return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions 
 ***********************************/

/* $begin sigaction */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}
/* $end sigaction */

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
	unix_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
	unix_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{ 
    if (sigfillset(set) < 0)
	unix_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
	unix_error("Sigaddset error");
    return;
}

void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
	unix_error("Sigdelset error");
    return;
}

int Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
	unix_error("Sigismember error");
    return rc;
}

int Sigsuspend(const sigset_t *set)
{
    int rc = sigsuspend(set); /* always returns -1 */
    if (errno != EINTR)
        unix_error("Sigsuspend error");
    return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */

/* $begin sioprivate */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b) 
{
    int c, i = 0;
    int neg = v < 0;

    if (neg)
	v = -v;

    do {  
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)
	s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}
/* $end sioprivate */

/* Public Sio functions */
/* $begin siopublic */

ssize_t sio_puts(char s[]) /* Put string */
{
    return write(STDOUT_FILENO, s, sio_strlen(s)); //line:csapp:siostrlen
}

ssize_t sio_putl(long v) /* Put long */
{
    char s[128];
    
    sio_ltoa(v, s, 10); /* Based on K&R itoa() */  //line:csapp:sioltoa
    return sio_puts(s);
}

void sio_error(char s[]) /* Put error message and exit */
{
    sio_puts(s);
    _exit(1);                                      //line:csapp:sioexit
}
/* $end siopublic */

/*******************************
 * Wrappers for the SIO routines
 ******************************/
ssize_t Sio_putl(long v)
{
    ssize_t n;
  
    if ((n = sio_putl(v)) < 0)
	sio_error("Sio_putl error");
    return n;
}

ssize_t Sio_puts(char s[])
{
    ssize_t n;
  
    if ((n = sio_puts(s)) < 0)
	sio_error("Sio_puts error");
    return n;
}

void Sio_error(char s[])
{
    sio_error(s);
}

/********************************
 * Wrappers for Unix I/O routines
 ********************************/

int Open(const char *pathname, int flags, mode_t mode) 
{
    int rc;

    if ((rc = open(pathname, flags, mode))  < 0)
	unix_error("Open error");
    return rc;
}

ssize_t Read(int fd, void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0) 
	unix_error("Read error");
    return rc;
}

ssize_t Write(int fd, const void *buf, size_t count) 
{
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
	unix_error("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) 
{
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
	unix_error("Lseek error");
    return rc;
}

void Close(int fd) 
{
    int rc;

    if ((rc = close(fd)) < 0)
	unix_error("Close error");
}

int Select(int  n, fd_set *readfds, fd_set *writefds,
	   fd_set *exceptfds, struct timeval *timeout) 
{
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
	unix_error("Select error");
    return rc;
}

int Dup2(int fd1, int fd2) 
{
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
	unix_error("Dup2 error");
    return rc;
}

void Stat(const char *filename, struct stat *buf) 
{
    if (stat(filename, buf) < 0)
	unix_error("Stat error");
}

void Fstat(int fd, struct stat *buf) 
{
    if (fstat(fd, buf) < 0)
	unix_error("Fstat error");
}

/*********************************
 * Wrappers for directory function
 *********************************/

DIR *Opendir(const char *name) 
{
    DIR *dirp = opendir(name); 

    if (!dirp)
        unix_error("opendir error");
    return dirp;
}

struct dirent *Readdir(DIR *dirp)
{
    struct dirent *dep;
    
    errno = 0;
    dep = readdir(dirp);
    if ((dep == NULL) && (errno != 0))
        unix_error("readdir error");
    return dep;
}

int Closedir(DIR *dirp) 
{
    int rc;

    if ((rc = closedir(dirp)) < 0)
        unix_error("closedir error");
    return rc;
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) 
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
	unix_error("mmap error");
    return(ptr);
}

void Munmap(void *start, size_t length) 
{
    if (munmap(start, length) < 0)
	unix_error("munmap error");
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size) 
{
    void *p;

    if ((p  = malloc(size)) == NULL)
	unix_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size) 
{
    void *p;

    if ((p  = realloc(ptr, size)) == NULL)
	unix_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size) 
{
    void *p;

    if ((p = calloc(nmemb, size)) == NULL)
	unix_error("Calloc error");
    return p;
}

void Free(void *ptr) 
{
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE *fp) 
{
    if (fclose(fp) != 0)
	unix_error("Fclose error");
}

FILE *Fdopen(int fd, const char *type) 
{
    FILE *fp;

    if ((fp = fdopen(fd, type)) == NULL)
	unix_error("Fdopen error");

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream) 
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
	app_error("Fgets error");

    return rptr;
}

FILE *Fopen(const char *filename, const char *mode) 
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
	unix_error("Fopen error");

    return fp;
}

void Fputs(const char *ptr, FILE *stream) 
{
    if (fputs(ptr, stream) == EOF)
	unix_error("Fputs error");
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream)) 
	unix_error("Fread error");
    return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    if (fwrite(ptr, size, nmemb, stream) < nmemb)
	unix_error("Fwrite error");
}


/**************************** 
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol) 
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
	unix_error("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) 
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
	unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) 
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
	unix_error("Bind error");
}

void Listen(int s, int backlog) 
{
    int rc;

    if ((rc = listen(s,  backlog)) < 0)
	unix_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
	unix_error("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) 
{
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
	unix_error("Connect error");
}

/*******************************
 * Protocol-independent wrappers
 *******************************/
/* $begin getaddrinfo */
void Getaddrinfo(const char *node, const char *service, 
                 const struct addrinfo *hints, struct addrinfo **res)
{
    int rc;

    if ((rc = getaddrinfo(node, service, hints, res)) != 0) 
        gai_error(rc, "Getaddrinfo error");
}
/* $end getaddrinfo */

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, 
                 size_t hostlen, char *serv, size_t servlen, int flags)
{
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv, 
                          servlen, flags)) != 0) 
        gai_error(rc, "Getnameinfo error");
}

void Freeaddrinfo(struct addrinfo *res)
{
    freeaddrinfo(res);
}

void Inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
    if (!inet_ntop(af, src, dst, size))
        unix_error("Inet_ntop error");
}

void Inet_pton(int af, const char *src, void *dst) 
{
    int rc;

    rc = inet_pton(af, src, dst);
    if (rc == 0)
	app_error("inet_pton error: invalid dotted-decimal address");
    else if (rc < 0)
        unix_error("Inet_pton error");
}

/*******************************************
 * DNS interface wrappers. 
 *
 * NOTE: These are obsolete because they are not thread safe. Use
 * getaddrinfo and getnameinfo instead
 ***********************************/

/* $begin gethostbyname */
struct hostent *Gethostbyname(const char *name) 
{
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL)
	dns_error("Gethostbyname error");
    return p;
}
/* $end gethostbyname */

struct hostent *Gethostbyaddr(const char *addr, int len, int type) 
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
	dns_error("Gethostbyaddr error");
    return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, 
		    void * (*routine)(void *), void *argp) 
{
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
	posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
    int rc;

    if ((rc = pthread_cancel(tid)) != 0)
	posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return) {
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0)
	posix_error(rc, "Pthread_join error");
}

/* $begin detach */
void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
	posix_error(rc, "Pthread_detach error");
}
/* $end detach */

void Pthread_exit(void *retval) {
    pthread_exit(retval);
}

pthread_t Pthread_self(void) {
    return pthread_self();
}
 
void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
    pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/
/* sem_init depricated in macOS
// not used anymore
void Sem_init(sem_t *sem, int pshared, unsigned int value) 
{
    if (sem_init(sem, pshared, value) < 0)
	unix_error("Sem_init error");
}
*/


void P(sem_t *sem) 
{
    if (sem_wait(sem) < 0)
	unix_error("P error");
}

void V(sem_t *sem) 
{
    if (sem_post(sem) < 0)
	unix_error("V error");
}

/****************************************
 * The Rio package - Robust I/O functions
 ****************************************/

/* rio_readn Robustly read n bytes (unbuffered)
 * 在网络编程中，因为较长的网络延迟，会导致系统调用read()返回不足值。
 * 1. 返回正数字节数
 *      1. 遇到EOF，返回不足值，但是不一定为0，可能是小于n的字节数目
 *      2. 返回足值n
 * 2. 出错会返回-1
 * 特点：
 * 1. 因为网络延迟，遇到read()返回除开EOF外的不足值，rio_readn()能够自动重启read()继续读取，
 * 2. read()如果被信号处理程序中断，rio_readn()能够自动重启read()，具有较好的移植性
 * 3. 对同一个描述符，可以任意交错调用rio_readn和rio_writen
 * 总结：能够处理网络延迟，并且碰到信号处理程序中断能够重启，因此相比read函数具有更好的鲁棒性
 * */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    while (nleft > 0) 
    {
    /*  系统调用read如果被信号中断，信号处理函数执行完成后，read不再继续，
     *  而是返回-1，并且设置错误代码errno == EINTR。
     *  系统调用read被中断通常不会导致文件描述符的状态或数据流的状态发生改变。
     *  也就是说，被中断的read调用不会影响文件描述符指向的文件或流的当前读取位置。
     *  这里遇到errno == EINTR的情况，会设置nread = 0，nleft保存不变，从而重新进入循环
     *  这里封装了read，能够自动重启中断的系统调用函数read，包装函数更具有鲁棒性
     * */
        if ((nread = read(fd, bufp, nleft)) < 0) 
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
                return -1;      /* errno set by read() */ 
        } 
        else if (nread == 0)    // 当read函数遇到EOF，会返回0，因此如果nread为0，表面遇到EOF
            break;              /* EOF */
        nleft -= nread;     // 遇到read()返回不足值
        bufp += nread;
    }
    return (n - nleft);         /* Return >= 0 */
}

/* rio_writen Robustly write n bytes (unbuffered)
 * 1. 出错返回-1
 * 2. 绝对不返回不足值
 * 
 * */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;
    while (nleft > 0) 
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) 
        {
            // 道理同rio_readn()
            if (errno == EINTR)  /* Interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            else if(errno == EPIPE)
            {
                /* homework 11.13 */
                fprintf(stderr, "Client already closed the connection prematurely.\n");
                break;  // 直接退出循环，返回n，避免进程直接终止
            }else
                return -1;       /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}


/* 
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 * 1. 返回读取的字节数：可能是一个不足值 
 *      1. 返回不足值：
 *          1. 返回0：read遇到了EOF
 *          2. 其他大于0的不足值：read读取了比n小的字节数，或者内部缓冲剩余字节数小于n
 *      2. 返回足值n
 * 2. 返回-1:表示遇到了非信号处理程序中断的其他错误
 * 3. 能够处理信号处理程序带来的中断并重新启动read()
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;
    while (rp->rio_cnt <= 0)    // 缓冲区中未读的字节树为小于等于0
    {  
        /* Refill if buf is empty 重新读取并填充内部缓冲区 */
	    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) 
        {
            /*  
            * 如果是被信号处理程序中断，read返回-1，并且设置全家错误变量为EINTR
            * 此时rp->rio_cnt = -1，会重新循环进行读取
            * 如果不是信号处理程序中断，则为其他错误，直接返回-1
            * */
            if (errno != EINTR)     // 如果不是EINTR，则表示为read并非信号中断，返回-1
                return -1;
            
        }
	    else if (rp->rio_cnt == 0)  // EOF导致read返回不足值0，直接返回0
	        return 0;
	    else        
	        rp->rio_bufptr = rp->rio_buf; // 成功读取了rp->rio_cnt个字节并设置rp->rio_bufptr
    }
    /* rp->rio_cnt可能远大于需要的n，但是也可能小于n（因为read读取了不足值）*/
    cnt = n;          // cnt取cnt和rp->rio_cnt的较小者
    if (rp->rio_cnt < n)   
	    cnt = rp->rio_cnt;
    /* 从内部缓冲rp->rio_bufptr拷贝cnt个字节到用户缓冲usrbuf */
    memcpy(usrbuf, rp->rio_bufptr, cnt);    
    rp->rio_bufptr += cnt;  // 更新rp->rio_bufptr位置，以备下次读取
    rp->rio_cnt -= cnt; // 更新缓冲区未读字节数量，以备下次读取
    return cnt; // 返回从缓冲区复制出来的字节数目
}

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 * 关联文件描述和一个读缓冲区并且重置缓冲
 */

void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;    // 关联缓冲区和文件描述符
    rp->rio_cnt = 0;    // 缓冲区未读字节数量
    rp->rio_bufptr = rp->rio_buf;   // 下一个未读字节位置
}

/*
 * rio_readnb - Robustly read n bytes (buffered)
 * 带内部缓冲的读函数（具有鲁棒性）特点：
 * 1. 从内部缓冲区复制n个bytes数据，如果内部缓冲区空了，能够自动填充
 * 2. 适合既包含文本又包含二进制数据的文件：使用rio_readnb读取二进制，使用rio_readlineb读取文本
 * 3. 对同一描述符和内部缓冲，rio_readlineb和rio_readnb可以交错使用
 * 返回值
 * 1. 返回-1，出现非信号中断的错误
 * 2. 返回正数字节数
 *      1. 返回不足值：因为遇到EOF，返回不足值，但是不一定为0，而是已经读取的小于n的字节数
 *      2. 返回足值n 
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    while (nleft > 0) 
    {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) 
                return -1;      // 遇到非信号处理程序中断的错误，rio_read中已经帮你处理了信号中断的错误
        else if (nread == 0)
            break;              /* 遇到EOF返回0 */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

/* 
 * rio_readlineb - Robustly read a text line (buffered)
 * 带缓冲区的读行函数（鲁棒性）
 * 从内部缓冲区复制一个文本行
 * 适合从文本文件每次读取一个文本行
 * 对同一描述符和缓冲，rio_readlineb和rio_readnb可以交错使用
 * 1. 返回-1，出现非信号中断的错误
 * 2. 返回0，遇到EOF
 * 3. 返回正数，文本行个数/maxlen-1
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    int n, rc;
    char c, *bufp = usrbuf;
    for (n = 1; n < maxlen; n++)    // 最多读取maxlen-1个字符，最后一个留给'\0'
    { 
        if ((rc = rio_read(rp, &c, 1)) == 1) 
        {
	        *bufp++ = c;
	        if (c == '\n')  // 提前读到换行符，说明本行已经结束
            {
                n++;
     		    break;
            }
	    }else if (rc == 0) 
        {
	        if (n == 1)
                return 0; // 遇到EOF，并且还没有读数据，直接返回0
            else
                break;   // 遇到EOF，并且已经读了一些数据，跳出循环
	    }else
	        return -1;	  // 遇到非信号处理程序中断的错误，rio_read中已经帮你处理了信号中断的错误
    }
    *bufp = 0;  // 使用NULL(0)结束文本行
    return n-1; // 实际读取的字节数目（不包括结尾的NULL）
}

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
    return rc;
} 

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, char *port) {
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    // connection type socket
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    // force using port number
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror(rc));
        return -2;
    }
  
    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) 
            break; /* Success */
        if (close(clientfd) < 0) { /* Connect failed, try another */  //line:netp:openclientfd:closefd
            fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
            return -1;
        } 
    } 

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* All connects failed */
        return -1;
    else    /* The last connect succeeded */
        return clientfd;
}
/* $end open_clientfd */

/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_listenfd */
int open_listenfd(char *port) 
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval=1;

    memset(&hints, 0, sizeof(struct addrinfo)); //  只能设置某些字段，只好先清空hints
    /* hints提供对getaddrinfo返回的listp指向的套接字链表更好的控制 */
    hints.ai_socktype = SOCK_STREAM;      // 对每个地址，规定只返回SOCK_STREAM套接字
    /*  hints.ai_flags 是掩码 */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // 返回的套接字用作被动监听的套接字，套接字地址为通配符，此时host为NULL
    hints.ai_flags |= AI_NUMERICSERV;            // 强制参数端口为数字
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    /* 遍历列表，找到套接字，直到成功 */
    for (p = listp; p; p = p->ai_next) {
        /* 直接用返回addrinfo中的数据结构作为参数调用socket函数 */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue;  // 失败了，继续链表中下一个套接字地址

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

        /* 将套接字描述符绑定到套接字地址，同样用addrinfo的值作为参数 */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;  // 绑定成功，跳出循环
        if (close(listenfd) < 0) 
        {   
            /* 绑定失败，关闭这个套接字描述符，继续下一个 */
            fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    freeaddrinfo(listp);    // 为了避免内存泄露，释放链表
    if (!p)     // p为空，意味着链表检查完，没有套接字地址绑定描述符成功
        return -1;
    
    /* 在监听套接字描述符上监听 */
    if (listen(listenfd, LISTENQ) < 0) // 出错的话返回-1
    {
        close(listenfd);    
	    return -1;
    }
    return listenfd;    // 返回监听描述符
}
/* $end open_listenfd */

/****************************************************
 * Wrappers for reentrant protocol-independent helpers
 ****************************************************/
int Open_clientfd(char *hostname, char *port) 
{
    int rc;
    if ((rc = open_clientfd(hostname, port)) < 0) 
	unix_error("Open_clientfd error");
    
    return rc;
}


int Open_listenfd(char *port) 
{
    int rc;
    if ((rc = open_listenfd(port)) < 0)
	unix_error("Open_listenfd error");
    return rc;
}

/* $end csapp.c */





