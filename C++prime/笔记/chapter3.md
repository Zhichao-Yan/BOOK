## 多维数组
-------------

### 多维数组初始化
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

#### 指针和多维数组
* 常用示例
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
* 指针变量和整数相加
    * 不是用指针变量里的地址直接加减这个整数
    * 整数的单位不是 byte 而是元素的个数
* 示例1
```C++
struct Test
{
    int Num;
    char *pcName; 
    short sDate; 
    char cha[2]; 
    short sBa[4];
}*p;
```
* p + 0x1 = 0x100000+sizof(Test)*0x1
* (unsigned long)p + 0x1 = (unsigned long)p + 0x1 一个无符号的长整型数加 上另一个整数
* (unsigned int*)p + 0x1 = 0x100000+sizof(unsigned int)*0x1

* 示例2
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
