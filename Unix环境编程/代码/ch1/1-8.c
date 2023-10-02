#include "apue.h"
#include <errno.h>
int main(int argc, char const *argv[])
{
	fprintf(stderr, "EACCES:%s\n", strerror(EACCES));
	errno = ENOENT;/* errno 属于全局变量*/
	perror(argv[0]);
	return 0;
}