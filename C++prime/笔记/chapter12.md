# 动态内存

* 使用动态内存的原因
    1. 不确定自己需要的元素数量大小 e.g. vector会先动态分配一块内存，大小不够再扩充，根据需求动态地分配
    2. 不知道所需要对象的准确类型
    3. 程序需要在多个对象之间共享数据  e.g. 数据放在共享内存，由用户控制该块内存的分配和消化，无论是是使用智能指针还是手动内存管理。

## 使用普通指针管理内存
### 动态变量
* 使用new动态分配和初始化对象：无法为分配的对象命名，只能返回指针
    * 分配方式
        * 默认情况下，默认初始化
            * `int *pi = new int;// 未定义的行为`
            * `string *ps = new string;// 初始化为空string对象`
        * 可以直接初始化
            * `int *pis = new int(1024);`
            * `string *ps = new string(10,'s');`
        * 值初始化
            * `string *ps = new string();// 值初始化为空`
            * `int *pi = new int();// int值初始化为0`
        * auto单一初始化器
            * `auto p1 = new auto(obj);`
    * new可以分配const对象：必须进行初始化
        * 一般类型必须显示初始化
        * `const int *pci = new const int(1024);`
        * 定义了默认构造函数的类类型可以隐式初始化
            * `const string *pcs = new const string;`
    * 分配内存空间失败：内存空间不足
        * 普通new`int *p1 = new int;`分配失败，抛出std::bad_alloc异常
        * 定位new`int *p2 = new(nothrow) int;`分配失败返回空指针，不抛出异常

* 使用delete表达式释放动态内存：销毁给定指针指向的对象，释放分配的内存空间
    * 使用要求
        * 必须delete一个指针
        * delete指针指向的对象必须是动态内存分配
        * 同一个指针不能多次delete
        * const对象指针的删除和普通对象指针删除一样

* 使用new和delete存在的问题
    1. 忘记释放内存，导致内存泄露：内存没有归还自由空间，其他的动态内存分配也不会使用这快内存，导致内存白白浪费使用机会。
    2. 使用已经释放的内存。
        * delete后指针地址存在，但是内存内容可能发生变化 e.g.内存块置0
        * 通常释放后将指针置为空可以避免错误的发生。但是可能有多个指针变量都指向了同一块内存，都应该置空。
    3. 同一动态内存多次释放。
    4. 未初始化指针并使用。

### 动态数组：

* 注意：尽可能使用标准容器，而不是动态分配的数组。
    * 不容易出现内存管理错误
    * 更好的实现效率

* new和数组
    * 格式：`int *pa = new int[size];`
    * 分配动态数组得到元素指针
    * 可以通过数组类型别名进行分配`typedef int arrrT[42];int *p = new arrT`
    * 初始化动态分配的数组
        * 默认初始化 `int *pia = new int[10];`
        * 值初始化`int *pia = new int();`
        * 列表初始化`int *pia = new int[10]{10,12,14,13,13,12};`
    * 不能用auto分配数组 `auto p2 = new auto{a,b,c}//错误`
    * 动态分配长度为0的数组是合法的，但是指针不能解引用，毕竟不指向任何元素

* delete和数组
    * delete []p;空括号是必须的，省略方括号出错
    * 数组的中的元素按逆序销毁

#### allocate 类
* 为啥需要allocate类？
* new和delete默认情况下会将对象构造和内存分配结合在一起，但这有时候并不必要
    * 分配了内存，但是我们往往不会一下子用到这么多对象。所以对象构造并没有必要

* 定义：将内存分配和对象构造分离，分配到内存是原始的，未构造的。
    * `allocate<string> a`定义了可以为string分配内存的对象
* 分配内存`p = a.allocate(n)`分配一段原始的未构造的内存，可以保存n个string对象,返回内存起始地址到p
* 释放内存`deallocate(p,n)`释放从p地址开始分配的能容纳n个string的内存块
* 构造对象`a.construct(p,args)`在指针地址p使用args参数构造string对象
* 析构对象`a.destroy(p)`在指针地址p析构对象
* 使用未构造的内存是没有意义的。
* 相应的拷贝和填充算法
    * uninitialized_copy(b, e, b2)
    * uninitialized_copy_n(b, n, b2)
    * uninitialized_fill(b, e, t)
    * uninitialized_fill_n(b, n, t)


## 使用智能指针管理动态内存
* 智能指针：行为类似常规指针，区别在于能够自动释放指向的对象
* 使用场景
    * 在异常发生和处理中使用智能指针确保资源得到释放
    * 在非良好定义类中：没有定义析构函数，使用智能指针管理内存

### 共享智能指针shared_ptr
* 使用方法
    1. 如果不初始化shared_ptr,它会变成空指针`shared_ptr<double> pd;`
    2. 使用new返回的指针初始化shared_ptr
        * `shared_ptr<int> p(new int(42))`必须直接初始化
        * `shared_ptr<int> p = new int(42)`错误，接受指针参数的智能指针构造函数是explicit
    3. p.reset()操作更新p的引用计数，将智能指针绑定到新的内存块
    ```C++
    if(!p.unique())
    {
        p.reset(new string("new string"));// 如果智能指针指向的内存块使用的用户不是唯一，为了避免改变字符串影响其他该内存使用用户，当前用户可以绑定到新的内存块，并且智能指针递减旧内存计数器
    }else
        *p = "new string";// 如果这块内存使用对象唯一，那么我们可以直接赋值，不害怕影响到其他用户
    ```
    4. **best practice**:使用make_share<>(args)来返回共享智能指针
        * 和模版的使用一样，<>知道指针指向的对象类型
        * 用参数构造给定类型的对象

* shared_ptr拷贝赋值
    * 计数器递增：
        * 初始化shared_ptr
        * 给函数传参数
        * 函数返回值
    * 计数器递减
        * 销毁指针
        * 赋予新的指针
* shared_ptr的析构函数
    * 递减计数器，如果计数=0，析构函数销毁对象并释放占有的内存

* **普通指针和智能指针注意点**
    1. 将智能指针绑定到普通指针，那么内存的管理责任就被转移，一旦这样做，我们不应该再使用内置指针访问shared_ptr指向的内存。使用内置指针来访问一个智能指针管理的对象是危险的，因为智能指针自动化管理内存，我们无法知道对象是否被摧毁了。
    ```C++
    int *x(new int(1024));
    process(shared_ptr<int>(x));
    int j = *x; //未定义，x是空悬指针
    ```
    2. 不要使用get返回的指针初始化另外一个智能指针
    ```C++
    shared_ptr<int> p(new int(1024));
    int *q = p.get();
    {
        shared_ptr<int> ptr(q);
    }// 程序块结束，ptr智能指针销毁，顺带把q的内存对象销毁了
    int foo = *q;// p已经是空悬指针，虽然有地址，但是不指向对象。之前的对象被消耗了
    // 智能指针 p和ptr是独立创建，计数器都是1，但是指向同一块内存，后面智能指针p是否时内存还会第二次delete
    // get函数的使用场景：不能使用智能指针的代码，并且确保不回delete指针
    ```
    3. 不要delete由get()返回的指针
    4. 不要用相同的内置指针初始化多个智能指针
    5. 非new分配的内存应该给智能指针传递删除器




### 独享智能指针unique_ptr
* unique_ptr也是能够自动释放内存对象，但是它一个人独占该块内存

* 使用 unique_ptr<int> p;
    * unique_ptr<int> p; //指针为空
    * 必须直接初始化`unique<int> p2(new int(42));`
    * p = nullptr 将p置空，释放该块动态内存，不将控制器转移，而是彻底丢弃
    * p.release() p放弃对该块内存的控制权，返回普通指针，并将p置空
    ```C++
    unique_ptr<string> p1(new string("hello"));
    unique_ptr<string> p2(p1.release());
    ```
    * reset
        * p.reset();// p指向的动态内存释放，将p置空
        * p.reset(q);// p释放指向的动态内存对象，指向内置指针q所在的内存
    * unique_ptr独占内存对象，无法拷贝赋值
        * 特例：可以拷贝或者赋值一个即将销毁的unique_ptr
            * 从函数返回的unique_ptr
    * best practice: make_unique<int>p(10)

* 删除器重载`unique_ptr<objT,delT> p(new objT,fcn);`


### 弱共享智能指针weak_ptr
* 定义：
    * 不控制所指向对象生存期的智能指针，即不会释放和绑定该对象。
    * 指向一个shared_ptr管理的对象
        * 不改变该shared_ptr引用计数
        * 不影响该shared_ptr释放对象

* 初始化为空 `weak_ptr<int> wp;//空指针`
* 创建 `auto p = make_shared<int>(42);weak_ptr<int> wp(p);`
* 共享计数器`wp.use_count()`和wp共享对象的shared_ptr个数
* 过期与否`wp.expired()`use_count()==0则返回true，否则返回false

* 访问对象：不能直接访问对象，使用lock()
    * 如果expired为空，返回空的shared_ptr
    * 如果expired不为空，返回指向和w共享对象的shared_ptr
```c++
if(shared_ptr<int> np = wp.lock())
{

}
```
* 为什么需要weak_ptr？？
* 虽然shared_ptr也有计数器use_count()和unique(),但是它们只限于该shared_ptr还存在的情况。
    * 当我们还未绑定shared_ptr指针时即shared_ptr<int> ptr,此时计数器`ptr.use_count()`为0,`ptr.unique()`不具有意义
    * 一旦我们绑定了，即shared_ptr<int> ptr = make_shared<int>(1024),`ptr.use_count()返回值必然大于等于1`，只有ptr存在时，我们才能调用`ptr.use_count()`,一旦这个时候释放，那么我们对ptr指向的对象将一无所知，无法试探情况，这个时候ptr已经不具有效力
* 我们需要一个weak_ptr只用于访问内存对象。它没有控制内存对象生存期的操作。不像shared_ptr一样偏重于共享属性。因此它被称为弱共享智能指针。


## 文本查询程序设计