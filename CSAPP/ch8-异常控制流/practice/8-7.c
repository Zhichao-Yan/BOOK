#include "csapp.h"

unsigned int snooze(unsigned int secs)
{
    unsigned int left = sleep(secs);
    printf("Slept for %u of %u secs\n",secs - left,secs);
    return left;
}

void signal_handle(int)
{
    return;
}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s <secs>\n",argv[0]);
        exit(1);
    }
    if (signal(SIGINT, signal_handle) == SIG_ERR)  //line:ecf:sigint:begininstall
	    unix_error("signal error");     
    int secs = strtoul(argv[1],NULL,0);
    snooze(secs);
    return 0;
}