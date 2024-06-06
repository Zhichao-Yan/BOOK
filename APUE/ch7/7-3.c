#include "apue.h"


static void exit1(void)
{
	printf("exit1\n");
}

static void exit2(void)
{
	printf("exit2\n");
}


int main(int argc, char const *argv[])
{
	if(atexit(exit2) != 0)
        err_sys("cann't register exit2");
	if(atexit(exit2) != 0)
	    err_sys("cann't register exit2");
	if(atexit(exit1) != 0)
	    err_sys("cann't register exit1");
    printf("main is done!!\n");
	return 0;
}