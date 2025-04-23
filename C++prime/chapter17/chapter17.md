



## 17.5 [IO库再探](../代码/chapter17/io.cpp)  

### 格式化输入输出
#### 控制输出格式
* IO对象状态
    * 条件状态
    * 格式状态：控制IO输出的格式化
        * 输出的整数是几进制
        * 浮点数的精度
        * 输出元素的宽度
        * 输出元素宽度太大如何填充/填充内容

> 标准库定义了操作符修改流的格式状态

* 操作符：修改流的格式状态。 如std::endl就是操作符，其输出换行符并且刷新缓冲区
    * 是一个函数或者一个对象
        * 接受参数的操作符在<iomanip>中。如setprecision(6)
        * 其他操作符在<iostream>中
    * 能作为输入输出运算符的运算对象，结果是返回流对象（这意味着可以连续操作流对象）
    * 受操作符改变的格式状态对后续IO都生效
    * 大多数成对存在：
        * 设置操作符：设置特殊状态
        * 复原操作符：恢复默认状态


* 控制布尔值的输出格式：
    * boolalpha
    * noboolalpha

* 控制整数的输出格式：
    * hex 十六进制
    * oct 八进制
    * dec 十进制

* 控制是否对非负数显示+
    * showpos
    * noshowpos
* 控制整数的进制格式是否显示：
    * showbase 打印时显示进制
        * 0x-十六进制
        * 0-八进制
        * 无前导字符-十进制
    * noshowbase 打印时不显示进制

* 控制输出格式中字母的大小写：
    * uppercase
    * nouppercase

* 控制浮点数
    * 浮点数精度：默认精度为6位，默认情况下为打印的数字总数（不包含小数点）
        * 通过IO对象的precision成员
            * cout.precision(int) 设置为int的精度，并返回旧的精度值
            * cout.precision() 显示当前的精度
        * 使用setprecision()操作符
            * 在头文件iomanip中
            * `cout << setprecision(3);`
    * 浮点数输出格式：根据数值确定默认格式
        * 定点十进制：其他默认格式
        * 科学技术法：数值非常大/小的默认格式
        * 十六进制
    * 没有小数的浮点数是否打印小数点：默认不打印小数点

* 指定浮点数记数法
    * 默认状态defaultfloat——根据要打印的值选择记数法
        * 精度值表示数字的总个数，包含小数点前
    * 强制改变：精度值表示小数点后的数字个数
        * scientific强制科学计数法
        * fixed 强制定点十进制
        * hexfloat强制十六进制

* 控制小数部分为0时小数点的打印 
    * showpoint
    * noshowpoint

* 控制列打印格式
    * setw()指定下一个输出数字或字符串的最小宽度:一次性，只决定下一个输出的宽度
        * 数字或字符串长度本身超过**指定宽度**可以按本身长度打印
        * 数字或字符串长度小于**指定宽度**造成留白
            * 对齐问题
                * left左对齐
                * right右对齐 默认右对齐
                * internal 在符号和值之间填充空白，即符号靠最左边显示
            * 填充问题
                * setfill代替默认的空格填充留白

#### 控制输入格式
* 忽略空白字符skipws-默认情况
    * 空白字符：空格符/制表符/换行符/回车符/换纸符
```C++
int main()
{
    char ch;
    while(cin >> ch)
        cout << ch;
}
// 输入a b c d
// 输出abcd
```
* 不忽略空白字符noskipws
```C++
int main()
{
    cin >> skipws;
    char ch;
    while(cin >> ch)
        cout << ch;
    cin >> skipws;
}
// 输入a b c d
// 输出a b c d
```

### 未格式化输入输出操作
> 可以把流当成一个无解释的字节序列处理

#### 单字节操作
* is.get(ch) 从输入流读取一个字节到ch，即使是空白字符，返回is
* is.get()从is读取到字节作为int返回
* os.put(ch) 将字符ch输出到os，返回os

* 有时我们需要试探性地读取一个字符来进行判断。符合要求就继续读取，不符合要求就退回字符。
* 退回方法：
    * is.peek() 偷瞄一下，将喵到的字节作为int返回，不会从流程中删除
    ```C++
    #include <iostream>
    #include <sstream>
    int main()
    {
        std::istringstream s1("Hello, world.");
        char c1 = s1.peek();
        char c2 = s1.get();
        std::cout << "Peeked: " << c1 << " got: " << c2 << '\n';
    }
    ```
    * is.unget() 输入流向后移动，刚刚读取的值又返回流中
    ```C++
    #include <iostream>
    #include <sstream>
    int main()
    {
        std::istringstream s1("Hello, world.");
        char c1 = s1.get();
        if (s1.unget())
        {
            char c2 = s1.get();
            std::cout << "Got: '" << c1 << "'. Got again: '" << c2 << "'.\n";
        }
    }
    ```
    * is.putback(ch) 退回从流中读取的最后一个值，接受的参数必须和最后读取的值相同,否则出错

* 问题？问什么is.peek()和is.get()返回int类型而不是char类型


#### 多字节操作

* 常用
    * is.get(sink,size,delim)
    * is.getline(sink,size,delim)
    * is.gcount() 确定最后一个未格式化输入操作读取了多少自己
        * 在peek()/unget()/putback()后使用is.gcount()会返回0

* 不常用
    * is.read(sink,size)读取最多size个字节，写入字符数组sink中，返回is
    * os.write(source,size)从source数组写入os，返回os 
    * is.ignore(size,delim)