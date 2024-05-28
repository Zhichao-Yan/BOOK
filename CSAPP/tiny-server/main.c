#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

char* strtok_r(char* start, const char* delim, char** save_start) 
{
    // 如果str为空，使用save_ptr作为起始点
    if (start == NULL) 
    {
        start = *save_start;
    }
    if (start == NULL)
    {
        return NULL;
    }
    // 找到第一个不属于delim的字符
    char* token = start + strspn(start, delim);
    if (*token == '\0') // 如果已经到达末尾了
    {
        *save_start = NULL;
        return NULL;
    }
    // 找到第一个在delim中的字符
    char *ptr = strpbrk(token, delim);
    // 如果没有找到
    if(ptr == NULL)
    {
        *save_start = NULL;
        return token;
    }else{
        ptr = '\0';
        ptr++;
        *save_start = ptr;
    }
    return token;
}

char** parse_query_string(char* str) 
{
    if (str == NULL) 
    {
        return NULL;
    }
    // 复制一个临时副本
    char temp[100];
    strcpy(temp,str);

    char *token;
    char **result = NULL;
    char *last = NULL; // 用于strtok_r的last参数

    // 首先计算参数的数量
    int count = 0;
    token = strtok_r(temp, "&", &last);
    while (token != NULL) 
    {
        count++;
        token = strtok_r(NULL, "&", &last);
    }

    // 分配足够的内存来存储指向每个参数的指针
    result = (char **)malloc((count + 1) * sizeof(char *));
    if (result == NULL) {
        perror("malloc failed");
        return NULL;
    }
    // 再次使用strtok_r分割字符串，并复制每个子字符串
    last = NULL;
    token = strtok_r(str, "&", &last);
    int i = 0;
    while (token != NULL) {
        result[i++] = token;
        token = strtok_r(NULL, "&", &last);
    }
    result[count] = NULL; // 以NULL结尾
    int j = 0;
    while(result[j] != NULL)
    {
        printf("%s\n",result[j]);
    }
    return result;
}

int main()
{
    char str[] = "value1=123&value2=2324&value3=13&value4=232224&yan=nsacs&yand=778";
    char **result = parse_query_string(str);
    free(result);
    return 0;
}


