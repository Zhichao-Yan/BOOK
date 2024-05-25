/* $begin sigint */
#include "csapp.h"

void sigint_handler(int sig) /* SIGINT handler */   
{
    Sio_puts("Caught SIGINT!\n");    /* safe output */
    _exit(0);                      /* safe exit */
}                                            

int main() 
{
    /* Install the SIGINT handler */         
    if (signal(SIGINT, sigint_handler) == SIG_ERR)  //line:ecf:sigint:begininstall
	    unix_error("signal error");                 //line:ecf:sigint:endinstall
    
    pause(); /* Wait for the receipt of a signal */  //line:ecf:sigint:pause
    return 0;
}
/* $end sigint */
