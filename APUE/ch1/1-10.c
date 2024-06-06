#include "apue.h"
#include <sys/wait.h>

static void sig_int(int);

int main(int argc, char const *argv[])
{
	char buff[MAXLINE] = {0};
	pid_t pid;
    if(signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");
	printf("%% "); /* 模拟命令行提示符 */
	while(fgets(buff, MAXLINE, stdin) != NULL){
		if(buff[strlen(buff) - 1] == '\n')
			buff[strlen(buff) -1] = '\0';
		if((pid = fork()) < 0)
		{
			perror("fork error");
			return -1;
		}else if(pid == 0)/* 子进程 */
		{
			if(execlp(buff, buff, (char *)0) < 0)/* execlp 启动的程序要求在PATH目录中能找到 */
			{
				perror("execlp error");
				return -1;
			}
			exit(0);
		}
		waitpid(pid, NULL, 0);
		printf("%%");
	}
	return 0;
}

void sig_int(int signo)
{
    printf("\ninterrupt!\n%%");
    return;
}