# Introduction

## 1.1 简单程序编写
### 函数构成
* 返回类型
* 函数名
* 行参列表
* 函数体
    * 左括弧
    * 右括号
    * return语句：结束函数的执行
        1. 记得return语句的`;`
        2. main函数return返回值表示状态。返回0表示成功，返回非0指示系统的程序错误类型
### 编译运行程序

* 编译器
GCC编译器套件：`g++-13`

* 编译命令
`g++-13 -o 1.1 1.1.cpp`

* 运行命令
`./1.1`

## 1.2 认识输入输出

### 标准输入输出对象
* cin 标准输入
* cout 标准输出
* cerr 输出错误和告警信息
* clog 输出程序运行的日志

> 系统通常将这些标准输入输出对象和程序运行的窗口联系起来
* cin：数据将从程序运行的窗口读入
* cout/cerr/clog：从程序运行的窗口显示出来


## 1.3 Comments in C++

* Comments help the human readers understand the program
    1. summarize an algorithm
    2. identify the purpose of variable
    3. clarify rather obscure segment of code
* Althoug Compiler ignores comments

* Two types of comments
    * // double slashes
    ```C++
    // Everything to the right of the slashes on the current line is ignored by the compiler
    // A comment of this kind can contain any text, including additional double slashes // 
    ```
    * pair
    ```C++
    /*
    * 
    * inherited from C language
    * These comments can include anything except itself
    * Our style is to begin each line in the comment with an asterisk *
    * thus indicating that the entire range is part of a multiline comment.
    */
    ```

* Program often contains a mixture of both comment forms
```C++
#include <iostream>
/*
* Comment pairs generally are used for multiline explanations
* Simple main function:
* Read two numbers and write their sum 
*/
int main() {
    // prompt user to enter two numbers ----sigle-line remark
    std::cout << "Enter two numbers:" << std::endl; 
    int v1 = 0, v2 = 0; // variables to hold the input we read 
    std::cin >> v1 >> v2; // read input --- half-line remark
    std::cout << "The sum of " << v1 << " and " << v2
                  << " is " << v1 + v2 << std::endl;
    return 0; 
    }
```

* Point need to know
    * Comment Pairs Do Not Nest


## 1.4 Flow control
* while
* for
* if 


## 1.5 Brief introduction to class
* Sales_item类
* 初识成员函数