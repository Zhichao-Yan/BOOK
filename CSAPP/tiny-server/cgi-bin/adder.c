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

int main(void) 
{
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE],content[MAXLINE], answer[MAXLINE];

    int n1=0, n2=0,head_only = 0;
    /* Extract the two arguments */
    // homework 11.10
    if ((buf = getenv("QUERY_STRING")) != NULL) 
    {
        p = strchr(buf, '&');
        *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = split(arg1);
        n2 = split(arg2);
    }
    if ((buf = getenv("HEAD_ONLY")) != NULL)
    {
        head_only = atoi(buf);
    }
    
    /* Make the response body */
    sprintf(content,"<html>\r\n");
    // head
    strcat(content,"<head>\r\n");
    strcat(content,"<meta charset=""utf-8"">\r\n<TITLE>adder</TITLE>\r\n");
    strcat(content,"<head/>\r\n");
    // body
    strcat(content,"<body>\r\n");
    strcat(content, "<h1> Welcome to Compute.com <h1/>\r\n");
    strcat(content,"<hr>\r\n");
    sprintf(answer, "<p><b>The answer is: %d + %d = %d <b/><p/>\r\n", n1, n2, n1 + n2);
    strcat(content,answer);
    strcat(content,"<body/>\r\n");
    strcat(content,"<html/>\r\n");
  
    /* Generate the HTTP response */
    printf("HTTP/1.0 200 OK\r\n");
    printf("Server: Tiny Web Server\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n");
    printf("Connection: close\r\n");
    printf("\r\n");

    if(head_only == 1)
        return 0;
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
/* $end adder */
