#include "apue.h"
#include <error.h>
#include <fcntl.h>

char buf[500000];

int main()
{
    int ntowrite,nwrite;
    char *ptr;
    ntowrite = write(STDIN_FILENO,buf,sizeof(buf));
    fprintf(stderr,"read %d bytes\n",ntowrite);
    set_fl(STDOUT_FILENO,O_NONBLOCK);
    ptr = buf;
    while(ntowrite > 0)
    {
        int errno = 0;
        nwrite = write(STDOUT_FILENO,ptr,ntowrite);
        fprintf(stderr,"nwrite = %d, errno = %d\n",nwrite,errno);
        if(nwrite > 0)
        {
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }
    clr_fl(STDOUT_FILENO,O_NONBLOCK);
    exit(0);
}