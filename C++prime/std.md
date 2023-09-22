* `#include<cmath>`
    * sqrt()
        * `sqrt(2.0f)`调用float版本
        * `sqrt(2.0l)`调用long double版本
        * `sqet(2.0)` 调用double版本


* `#include <string>`
    * `istream& getline ( istream &is , string &str , char delim )`
        * 遇到这个字符delim停止读入,系统默认该字符为'\n'
    * `str.c_str()` 将string换成c风格字符串

* `#include <iostream>`
    * `istream& getline (char* s, streamsize n )｜istream& getline (char* s, streamsize n, char delim )`  
        * 从输入流读取未格式化的流存储c类型字符串s中
        * 要么读够n-1个字符停止读取，要么碰到delim后停止读取
        * 遇到这个字符delim停止读入，在前面一种重载形式里delim默认是'\n'
        * 碰到delim会从流中抽取出来丢弃，不会写到s中
        * 读取到end-of-file，函数给输入流设置eofbit flag
        * '\0'会自动添加到末尾
    * `istream& get (char* s, streamsize n)｜istream& get (char* s, streamsize n, char delim)` 
        * 基本和getline相同
        * 读到delim字符会被保留在流中不读出来
        * '\0'会自动添加到末尾

*  `#include <cstring>`
    * `const char * strchr( const char * str, int character );` 找到第一个字符的位置
    * `const char * strrchr ( const char * str, int character );`找到最后一个字符的位置
    * `char * strcpy ( char * destination, const char * source );`复制字符串
    * `char * strncpy ( char * destination, const char * source, size_t num );`复制n个字符
    * `const char * strstr ( const char * str1, const char * str2 ); ` 在str1中查找str2字符串
    * `char * strcat ( char * destination, const char * source );` 剪接字符串
    * `size_t strlen ( const char * str );`返回字符串长度
    

* `#include <cassert>`
    * `assert(arg = 2)` 定义的宏函数，用于debug诊断

* `#include <cctype>`
    * `isupper(c)`
    * `islower(c)`
    * `toupper(c)`
    * `tolower(c)`
    * `isdigit(c)`
    * `isalpha(c)`
    * `isalnum(c)`
    * `isspace(c)`
