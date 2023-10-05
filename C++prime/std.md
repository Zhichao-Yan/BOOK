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


* `#include <pthread.h>`

* `#inlcude <cstdlib.h`
    * `void qsort(void* base, size_t num,size_t size,int (*compar)(const void*,const void*));`
    * 动态分配
        * `void* malloc (size_t size);`
            * Allocates a block of size bytes of memory
            * memory is not initialized, remaining with indeterminate values
            * If size is zero, the return value depends on the particular library implementation,but shall not be dereferenced
        * `void* realloc (void* ptr, size_t size);`
            * Changes the size of the memory block pointed to by ptr.
            * The content of the memory block is preserved 
            * if ptr is a null pointer, the function behaves like malloc
        * `void* calloc (size_t num, size_t size);`
            * Allocates an array of num elements, each of them size bytes long
            * initializes all its bits to zero.
            * If size is zero, the return value depends on the particular library implementation,but shall not be dereferenced
        * `void free (void* ptr);`
    * 随机数
        * `void srand (unsigned int seed);` usually `srand (time(NULL));`
        * `int rand (void);`
        Returns a pseudo-random integral number in the range between 0 and RAND_MAX.
        * `RAND_MAX`
        This value is library-dependent, but is guaranteed to be at least 32767 on any standard library implementation
    * 字符串——》数值
        * 带指向字符指针的指针，指向数值后面的第一个字符
            * `float strtof (const char* str, char** endptr);`
            * `double strtod (const char* str, char** endptr);`
            * `long double strtold (const char* str, char** endptr);`
            * `long int strtol (const char* str, char** endptr, int base);`
            * `long long int strtoll (const char* str, char** endptr, int base);`
            * `unsigned long int strtoul (const char* str, char** endptr, int base);`
            * `unsigned long long int strtoull (const char* str, char** endptr, int base);`
        * 直接解析
            * `double atof (const char* str);`
            * `int atoi (const char * str);`
            * `long int atol ( const char * str );`
            * `long long int atoll ( const char * str );`
    * 数值算法
        * 绝对值
            * `long long int llabs (long long int n);`
            * `long int labs (long int n);`
            * `int abs (int n);`
        * 除数取余
            * `div_t div (int numer, int denom);`
            * `ldiv_t ldiv (long int numer, long int denom);`
            * `lldiv_t lldiv (long long int numer, long long int denom);`