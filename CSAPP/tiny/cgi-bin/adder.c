/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int split(char *token)
{
    int value;
    if (token != NULL) {
        token = strtok(token, "="); // 分割出"value1"
        if (token != NULL) {
            token = strtok(NULL, "="); // 获取"123"
            if (token != NULL) {
                value = atoi(token); // 将字符"123"转换为整数
            }
        }
    }
    return value;
}
int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE], answer[MAXLINE];
    int n1=0, n2=0;
 

    /* Extract the two arguments */
    // homework 11.10
    if ((buf = getenv("QUERY_STRING")) != NULL) {
        p = strchr(buf, '&');
        *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = split(arg1);
        n2 = split(arg2);
    }

    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    strcat(content, "THE Internet addition portal.\r\n<p>");
    //sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    //strcat(content,"The answer is: ");
    sprintf(answer, "The answer is: %d + %d = %d\r\n<p>", 
	    n1, n2, n1 + n2);
    strcat(content,answer);
    strcat(content,"Thanks for visiting!\r\n");
    //sprintf(content, "%sThanks for visiting!\r\n", content);
  
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
/* $end adder */
