## String标准库
* 注意：**字符串字面值并不是string对象**，属于不同的类型
* string定义和初始化   
    * `string s1;`
    * `string s2(s1);`
    * `string s2 = s1;`
    * `string s3("value");`
    * `string s3 = "value";`
    * `string s4(n,'c');`

* string操作
    * 输入输出操作
        * `os << s`
        * `os >> s`
        * `getline(cin,s);`
    * 数量信息
        * `s.empty();`
        * `s.size();`
        * `string::size_type`
    * 比较操作:`<`/`>`/`<=`/`>=`/`==`/`!=`
    * 赋值操作`string s1(10,'A'); string s2; s2 = s1;`
    * 加法运算`+`/`+=` 运算符两边至少要有一个string对象

* 处理string对象中的字符
    * 处理每个字符：范围for语句
    * 处理部分字符：[]运算符
    * 迭代器


* 字符串相关
    * 字符数组表示字符串
        * `char s3[] = {'C', 'O', 'M','\0'}; // 必须显式添加空白字符`
        * `char a3[] = "C++"; // 字符串字面值会被拷贝到a3，自动添加空白字符，a3长度为4`
    * 字符指针表示字符串
        * `const char *s = "helloworld";`
        * `char *s = "helloworld"; // 错误，const char*不可以转char*`
        * `char s3[] = {'C', 'O', 'M','\0'}; char *s = s3;`
    * string对象返回C风格字符串
        * `const char *s = string("nihao");`


## vector标准库

* 定义和初始化
    * 默认初始化`vector<T> v1;`
    * 拷贝初始化
        * `vector<T> v2(v1);`
        * `vector<T> v2 = v1;` 
    * 直接初始化
        * `vector<T> v3(n,value);`
        * `vector<T> v3(n);`
    * 列表初始化
        * `vector<string> v1{"a", "an", "the"};`
        * `vector<string> v2 = {"a", "an", "the"};`
* 元素添加删除
    * `v.push_back(t);`
* vector数量性质
    * `v.empty();`
    * `v.size();`
* 访问vector元素
    * 范围for语句
    * 下标运算符[]
    * 迭代器操作
* 比较操作
    * `==`| `!=`
    * `>`|`<`|`>=`|`<=`

## 迭代器
* 迭代器类型
    * const_iterator
    * iterator
* 使用迭代器
    * 获取迭代器
        * v.begin()
        * v.end()
    * 访问运算符
        * *iter 
        * iter->mem
    * 移动迭代器 ++iter/--iter
    * 迭代器相等关系 `==`|`!=`



# 数组
## 一维数组
* 数组相比vector：大小固定，相对没那么灵活
* 数组类型
    * 元素个数
    * 元素类型
* 数组特点
    * 不允许auto由初始值列表推断元素类型
    * 数组的元素为对象，不存在引用的数组，因为引用不是对象

* 数组初始化：列表初始化

* 数组不允许拷贝赋值
```C++
int a[] = {1,2,3};
int a1[] = a;//错误，不允许拷贝初始化
a1 = a;// 错误，不允许赋值
```
* 理解复杂的数组声明
    * `int *ptrs[10];`包含10个指针的数组
    * `int (*Parray)[10] = &arr;` 指向含有10个int元素数组的指针
    * `int (&arrRef)[10] = arr;` 对含有10个int元素数组的引用

* 数组元素的访问
    * 范围for语句
    * 下标运算符：注意不要超出界限，防止越界

* 指针和数组
    * 使用数组名相当于使用数组首元素的地址
        ```C++
        string nums[] = {"one", "two", "three"};
        string *p = &nums[0];
        string *p2 = nums; // p2和p相等
        ```
        * 使用auto推断得到类型是指针而不是数组
        `int ia[] = {0,1,2,3,4,5,6,7,8,9}; auto ia2(ia); //ia2s是int指针类型`
        * 使用decltype推断得到类型是数组
        `decltype(ia) ia3 = {0,1,2,3,4,5,6,7,8,9}; // 返回10个整数数组作为的类型`

    * 指针作为迭代器 `int arr[] = {0,1,2,3,4,5,6,7,8,9};`
        * 首指针
            * `int *p = arr;` 自己获取
            * `int *beg = begin(arr);` 使用标准库函数返回
        * 尾后指针
            * `int *e = &arr[10];` 自己获取
            * `int *last = end(ia); `使用标准库函数返回 

## 多维数组

### 多维数组初始化
* 多维数组初始化
    1. 每一行都用花括号初始化
    ```C++
    //three elements; each element is an array of size 4
    int ia[3][4] = {
        {0, 1, 2, 3},// initializers for the row indexed by 0
        {4, 5, 6, 7}, // initializers for the row indexed by 1
        {8, 9, 10, 11} // initializers for the row indexed by 2 
        };
    ```
    2. 内层嵌套并非必要， `int ia[3][4] = {0,1,2,3,4,5,6,7,8,9,10,11};`
    3. 显式初始化每行的首元素，其他未列出的元素默认初始化`int ia[3][4] = {{ 0 }, { 4 }, { 8 }};`
    4. 初始化第一行4个元素，其他初始化为0 `int ix[3][4] = {0, 3, 6, 9};`


### 多维数组下标引用
1. 如果下标运算符数量和数组维度一样，表达式结果是给定类型的元素 `ia[2][3] = arr[0][0][0]`
2. 如果下标运算符数量比数组维度小，则表达式结果将是给定索引出的一个内层数组 `int(&row)[4] = ia[1]含有4个整数数组的引用`

### 多维数组的类型别名
1. `using int_array = int[4];` 新标准
2. `typedef int int_array[4];` C兼容

### 多维数组和指针
* 使用多维数组的名字时，会自动将其转换成指向数组首元素的指针

1. 常用示例
```C++
int ia[3][4];
//使用多维数组的名字时，自动将其转换成指向数组首元素的指针
//ia的 数组首元素 是一个 长度为4 的一维数组
//此处等价于 int(*p)[4] = &ia[0];
int(*p)[4] = ia;//p是一个指针变量，保存着一个 长度为4的数组 的指针
p = &ia[2];// p是一个指针变量，保存多维数组ia的末尾元素（长度为4的一维数组）指针
for (auto p = ia; p != ia + 3; ++p) {
// 令p保存指向ia的的第一个元素的指针（元素为长度4的一维数组）
// *p就代表 获得p指针变量指向的 长度为4的一维数组，直接使用*p 相当于使用长度为4的一维数组的名字
// q = *p  （此处令指针q指向数组*p的第一个元素）
    for (auto q = *p; q != *p + 4; ++q) 
             cout << *q << ’ ’;
        cout << endl;
}
```
2. 常用示例
```C++
int main()
{
    int a[4]={1,2,3,4};
    int *ptr1=(int *)(&a+1);//指向 a 数组后面的内存单元，&a+1 表示向后移 16 个存储单元
    int *ptr2=(int *)((int)a+1);//表示 a 的存储单元的地址增加一个字节 
    printf("%x,%x",ptr1[-1],*ptr2);//ptr1[-1]其实指向的是a数组的最后一个单元int整数
    // *ptr2 则表示 a 数组的地址后移一个字节之后的 4个字节所存储的int整数
    return 0;
}
```
