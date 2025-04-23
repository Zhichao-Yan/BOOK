# 特殊工具和技术

## 19.1 控制内存分配

-----------------
## 19.2 运行时类型识别

> RTTI:Run-Time Type Identification
> 其主要功能的实现由两个运算符完成
> * dynamic_cast 运算符：将基类的指针或引用安全地转换成派生类的指针或者引用
> * typeid运算符：用于返回表达式的类型

### Question：为什么我们需要RTTI运行时类型识别？？？

1. 常见的需求：通过**基类指针或者引用** 执行某个派生类的函数（操作）
    * [如果这个函数是虚函数](../代码/chapter19/virtual.cpp)
        * 基类指针或引用绑定派生类对象
        * 编译器通过对象的动态类型自动选择正确的函数版本。

    * 如果这个函数不是虚函数（无法用虚函数定义功能）导致无法使用虚函数
        * [不进行类型转换](../代码/chapter19/notvirtual.cpp)：
            * 这个函数在子类中被覆盖 
                * 结果：只能调用父类版本该操作函数。
            * 这个函数只是在子类中才被添加进来，父类中并没有。
                * 结果：编译出错
        * 进行类型转换：将基类指针引用---》派生类指针引用
            * [显式强制类型转换](../代码/chapter19/explicit-cast.cpp)
                * 非常危险
                * 类型不要求是多态类型，均可以强制类型转换
            * [dynamic_cast转换](../代码/chapter19/dynamic-cast.cpp)
                * 前提：类型必须是多态类型
                * 根据动态类型进行转换
                * 比较安全的转换，有可能转换失败，但是有安全机制处理转换失败。



-------
### 19.2.1 dynamic_cast运算符

* 前提：[转换类型必须是多态类型，否则出错](../代码/chapter19/dynamic-cast1.cpp)
    * 结果：编译出错 `error: cannot 'dynamic_cast' '& person' (of type 'class Person*') to type 'class Student*' (source type is not polymorphic)`

* 使用说明:
* 调用方式：
    1. `dynamic_cast<type*>(e)` e必须是指针
    2. `dynamic_cast<type&>(e)` e必须是左值
    3. `dynamic_cast<type&&>(e)` e必须不是左值
* 类型要求：
    1. e类型是type的公有派生类
    2. e类型是type的公有基类
    3. e就是是type目标类型

* 转换结果
    * 成功：e类型内存保存的实际对象类型应该比type类型的派生层次低--安全的向下转换
    * 失败：
        * 转换目标是指针类型：返回nullptr指针
        * 转换目标是引用类型：抛出bad_cast异常

------
### 19.2.2 typeid运算符

* [typeid(e)](../代码/chapter19/typeid.cpp)
    * e为任意表达式或者类型的名字
        * 表达式`typeid(*p)`
            * 类型含有虚函数，对表达式求值
                * 如果指针p指向类型有虚函数，*p将在运行时求值，因此p必须有效，否则（例如p是空指针）抛出bad_typeid异常
            * 类型没有虚函数，返回静态类型（编译器无需求表达式值也能知道静态类型）
                * 类型没有虚函数，没有动态检查，p可以是无效指针
        * 类型 
    * 返回结果为type_info类型的常量对象的引用

### 19.2.4 type_info类

* type_info类
    * 没有的操作
    *   没有默认构造函数-》无法默认生成type_info类型对象
    *   拷贝移动构造函数定义为删除-》无法借由type_info对象拷贝移动生成另外一个对象
    *   赋值运算符定义为删除-》不能为为type_info对象赋值
    *   创建type_info对象的唯一途径是typeid运算符。
    * 有的操作：
        * 虚析构函数-》可以作为基类生成派生类
        * t1 == t2
        * t1 != t2
        * t.name()
            * 返回C风格字符串表示类型名字
            * 返回值因为编译器的不同而不同
            * 不同类型返回字符串必须不同

* type_info实现：
    * 每一个有虚函数的类对象有一个虚函数表指针
    * 虚函数表指针指向该类的虚函数表
    * 虚函数表的第一个表项指向type_info对象

        

## 枚举类型

* 枚举 
    * 枚举类型 `enum class/enum`
        * 限定作用域 `enum class open_modes{ input,output,append};`
            * 在枚举类作用域外不可访问，通常带访问作用域符号使用：`open_modes::input`
            * 不可以转换成整型`int j = input;//错误`
        * 非限定作用域  `enum open_modes{input,output,append};` 可以**省略关键字class**
            * 可以转换成整型`int i = output;`
            * 枚举成员作用域和枚举类型作用域相同`enum color {red,green,yellow}`
                * 显式访问枚举成员`color eye = color::green;`
                * 隐式访问枚举成员`color hair = red;`
            ```C++
            enum color {red,green,yellow};
            enum class peppers {red,green,yellow};
            //enum stoplight {red,green,yellow}; 错误!stoplight枚举成员的作用域和color枚举成员作用域相同，造成冲突
            // peppers p = green; 错误，green，当前作用域默认选取color类型的成员，导致类型不匹配错误
            peppers p = peppers::green;//正确
            ```
    * 枚举成员:花括号里面
        * 枚举值：
            * 枚举值默认从0开始依次加1
            `enum open_modes{input,output,append};// input = 0,output = 1, append =2 从9开始依次+1`
            * 可以认为指定枚举成员的值
                * 提供的值可以不唯一
                `enum open_modes{input = 10,output = 16,append = 16}//枚举值可以不唯一`
                * 没有显示提供，则默认当前枚举值 = 之前枚举值 + 1
                `enum open_modes{input = 10,output,append}//此时output = 11，append = 12`
                * 枚举成员是const
                    * 初始化枚举成员的初始值必须是常量表达式
                    * 枚举成员本身是一条常量表达式，可以用在任何需要常量表达式的地方
                        * ~~定义枚举类型的constexpr变量~~
                        * ~~作为非类型模版形参~~ 
                        * ~~作为case标签~~
                        * ~~初始化枚举类型的静态数据成员~~

* 初始化enum对象或者赋值，必须使用该枚举类型的一个成员或者该类型的另外一个对象
    * 整数不能作为enum类型形参的实参
* 枚举类型实参可以传递给整型形参：只能是非限定类型,限定作用域不可以传递
    * enum的值可以提升成int或者更大的整型
* enum实际上是由某种整数类型表示
    * 可以指定枚举成员的整数类型（**C++11标准**）
        * **注意** 如果某个枚举成员的值超出了该类型能够容纳的范围，程序会出错
    * 潜在类型/默认类型/隐式指定
        * 限定作用域：默认类型是int，这是隐式指定
        * 不限定作用域：不存在默认类型，但是潜在类型足够大

* 枚举类型enum前置声明
    * 声明中必须指定枚举成员的类型
        * 不限定作用域枚举类型 `enum intValues:unsigned long long;` 必须指定类型
        * 限定作用域枚举类型 `enum class open_modes;` 因为潜在类型为int，可以不指定
    * enum的所以声明和定义必须一致
        * 上下午不能先声明一个不限定作用域枚举类型，再声明一个同名的限定作用域的枚举类型
        ```C++
        // 错误
        enum intValues;
        enum class intValues;// 潜在类型为int
        ```
        * 所有声明和定义中成员大小必须一致
        ```C++
        // 错误
        enum class intValues;// 潜在类型为int
        enum intValues:long;//指定类型为long
        ```

### 限定作用域和非限定作用域枚举类型区别
| 限定 | 非限定 | 
| :-----:| :----: | 
| 类作用域外不可访问 | 枚举成员作用域和枚举类型作用域相同| 
| 不可以转换成整型`int j = input;` | 可以转换成整型`int i = output;` | 
| 默认类型是int，这是隐式指定 | 不存在默认类型，但是潜在类型足够大|
| 前置声明不要求指明类型 | 前置声明必须指定类型|

------
