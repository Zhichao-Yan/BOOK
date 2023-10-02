#include "apue.h"
#define BUFFSIZE 4096
int main(int argc, char const *argv[])
{
	int n;
	int buff[BUFFSIZE] = {0};

	while((n = read(STDIN_FILENO, buff, BUFFSIZE)) > 0)/* 行缓冲，输入换行符才会真正读入 */
		if(write(STDOUT_FILENO, buff, n) != n)
			perror("write error");
	if(n < 0)
		printf("read error\n");

	return 0;
}

