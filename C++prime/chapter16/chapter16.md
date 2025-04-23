# 模版和泛型编程

## 定义模版
* Question1: 为什么需要模版
* Answer1:
    1. 对于功能相似，只是类型不同，如果不得不定义完全一样的函数体，是非常繁琐的
    2. 在编写程序时我们就要确定类型---这是脱离实际的，很多时候我们无法预计使用何种类型

* Question2:什么时候用模版？什么时候用继承
* Answer2:
    * 模版的使用：对象的类型不会影响函数的行为
    * 继承的使用：对象的类型影响函数的行为
    
* Question3:模版应该被定义在哪？
* Answer3:模版类和其中的成员函数应该被定义在同一个头文件中，而不像普通的类一样分开定义

### 函数模版

* 模版参数：表示在类和函数定义中用到的类和值。隐式或者显示地指定模版实参，绑定到模版参数上
    * 类型参数
        * 功能
            * 指定返回类型
            * 指定形参类型
            * 变量声明
            * 类型转换
    * 非类型参数：常量表达式，编译时能够确定
        * 整型
        * 指向对象或者函数的指针或引用
            * 绑定到指针或引用上面的对象必须具有静态生存期：编译时必须确定类型
                * 局部变量和动态分配的对象不可以作为实参
* 模版实例化：
    * 显式实例化
    * 隐式实例化：编译器推断出来的模版参数进行实例化

* inline和constexpr：模板参数列表之后，返回类型之前

* 编写类型无关的代码：模板程序应该尽量减少对实参类型的要求，让这个模版能够尽可能用于更多的类型
    * 如果有类型没有定义>运算符，因为其中的代码使用>运算符，那么可能就无法使用该类型

* 模版在工程中放哪里？
    * 函数模板和类模板成员函数的定义道常放在头文件中

* 模版的编译
    * 模版定义时不生成代码，模版被实例化（显式实例化）和使用时（隐式实例化），编译器才生成代码
        * 影响如何组织代码
            * 模板的头文件通常既包括模版的声明也包括模版的定义
            * 函数模板和类模板成员函数的定义道常放在头文件中
        * 影响何时获知模版内代码的编译错误
            1. 第一阶段：编译模版本身，编译器可以检查语法错误
            2. 第二阶段：模版使用，编译器检查实参数目是否正确，查参数类型是否匹配
            3. 第三阶段：模版实例化时，编译器可以发现类型相关的错误
### 类模版

* 类模版概念： 生成类的蓝图


* 类模版的实例化：
    * 实例化的方式
        * 隐式实例化：不可以，编译器不能推断类模版的参数类型
        * 显式实例化：在<>中指定类模版的实参，必须显式实例化
    * 实例化的原则：**类和成员函数只有在被使用到才会对其进行实例化** 
        * 所以即使某种类型不能完全符合模版操作的要求，也可以用来实例化，只要不使用对应操作即可
    * 实例化的结果
        * 每一个实例都是一个独立的类，与使用相同模版定义的其他类没有关联


#### 类模版定义
* 模版类名书写规范
    * 在类模版定义中使用其他的模版类
        * 通常不将一个实际类型或值的名字用作其模板实参。
        * 而是将自己的模板参数当作被使用模板的实參。
        * 参见类模版Blob<T>中的`std::shared_ptr<std::vector<T>> data;`
    * 在类模版定义内使用本模版的类名：可以简化模版类名的书写，可以直接写模版名而忽略模版实参
    ```C++
    BlobPtr& operator++(); // equal to BlobPtr<T>& operator++();
    BlobPtr& operator--();  //equal to BlobPtr<T>& operator--();
    BlobPtr operator++(int);
    ```

    * 在类模版定义外使用本模版的类名：不可以省略模版实参
    ```C++
    template <typename T>
    BlobPtr<T> BlobPtr<T>::operator++(int)
    {
    // no check needed here; the call to prefix increment will do the check 
        BlobPtr ret = *this; // 在作用域内，不需要使用 BlobPtr<T> ret = *this
        ++*this; // advance one element; prefix ++ checks the increment 
        return ret; // return the saved state
    }
    ```
    * 别名
        * 模版类的别名：实例化的类模版叫模版类型，给该模版类型起个别名 `typedef Blob<string> StrBlob;`
        * 类模版的别名：
            * 不固定模版参数
            ```C++
            template <typename T> using twin = pair<T,T>;
            twin<string> authors;// authors是一个pair<string,string>
            twin<int> win_loss;// win_loss是一个pair<int,int>
            twin<double> area;// area是一个pair<double>
            ```
            * 可以固定一个多个模版参数
            ```C++
            // template <typename T> pair<T,T>是一个类模版
            // 那么partNo是pair类中的一部分（一族），只不过它的第二个参数固定为unsigned
            // 使用partNo需要指定第一个参数
            template <typename T> using partNo = pair<T, unsigned>;
            partNo<string> books; // books is a pair<string, unsigned> 
            partNo<Vehicle> cars; // cars is a pair<Vehicle, unsigned> 
            partNo<Student> kids; // kids is a pair<Student, unsigned>
            ```

* 定义类模版的成员函数
    * 可以模版类内定义成员函数，默认为内联函数
    * 可以在模版类外定义成员函数，但是必须说明属于哪一个类
    ```C++
    template <typename T>
    ret-type Blob<T>::member-name(parm-list)
    ```

* 定义类模版的友元
    * 类模版中包含非模版友元
        * 类模版中包含非模版友元：友元可以访问所有模版实例-----> **多对一关系**
        ```C++
            template <typename T> class C2 { 
                friend class Pal3; // 非模版，Pal3是C2所有实例的友元
            };
        ```
        * 类模版自己的类型参数作为友元 --------> **一对一关系**
        ```C++
        template <typename Type> class Bar { 
            friend Type; // 将访问权限授予用来实例化Bar的类型
        };        
        ```
    * 类模版中包含模版友元
        * 相同类型实例化的模版类之间的友元 ------> **一对一关系**
        ```C++
        template <typename T> class Blob; // bool operator==()的声明需要认识Blob模版

        template <typename T> class BlobPtr; // Blob模版需要前置的BlobPtr友元声明，不然不认识
        bool operator==(const Blob<T>&, const Blob<T>&);// Blob模版需要前置的Bbool operator==()声明，否则不认识
        template <typename T1> class Blob {
            // 友元的声明用Blob模版的形参T1作为实参数
            friend class BlobPtr<T1>; // BlobPtr模版是友元
            friend bool operator==<T1> // 相等运算符是友元
                (const Blob<T1>&, const Blob<T1>&); 
        };
        Blob<char> ca; // BlobPtr<char> 和operator==<char> 都是Blob<char> ca的友元
        ```
        * 友元类模版Pal2的每一个实例都是类模版C2每一个实例的友元---->**多对多关系**
        ```C++
        template <typename T> class C2 { 
            template <typename X> friend class Pal2;
        };
        ```
    * 非模版类中包含非模版友元：即普通的友元，不涉及模版概念的友元定义。----> **一对一关系**
    * 非模版类中包含模版友元
        * 该非模版类作为模版实参----->**一对一的关系**
        ```C++
        template <typename T> class Pal;// 将Pal模版的一个特例作为友元需要前置声明
        class C { 
            friend class Pal<C>;  // 用C实例化的模版类Pal是C的一个友元
        }
        ```
        * 该非模版类不作为模版实参---->**一对多的关系**
        ```C++
        class C { 
            template <typename T> friend class Pal2; // Pal2的所有实例都是C的友元，这个时候无需前置声明
        };
        ```



* 定义类模版的静态成员
    * 对于任意实例化Foo的类型X：Foo<X>类型对象共享ctr对象和count函数
        * Foo<X>::count()静态成员函数
        * Foo<X>::ctr静态成员变量
    ```C++
    template <typename T> 
    class Foo { 
    public:
        static std::size_t count() { return ctr; }
        static std::size_t ctr;
    };
    ```
    * 类模版的每个static数据成员有且仅有一个定义
    ```C++
    template <typename T>// 需要包含模版参数列表
    size_t Foo<T>::ctr = 0;// ？？？为什么用0初始化，如果不是整型呢？或是自定义的类型呢？？？
    ```
    * 类模版的静态成员函数只有在使用时才会实例化
    * 类模版**静态成员**的访问：
        * 类和作用域访问符:`auto ct = Foo<int>::count();`
        * 通过类对象访问：`Foo<int> fi;ct = fi.count();`



### 模版参数
* 模版参数命名:参数命名可以使用任何名字
`template <typename T> == template <typename Foo>`

* 模版参数的作用域
    1. 会隐藏外层作用域声明的名字
    2. 在模版内不能重用模版参数
    ```C++
    typedef double A;
    template <typename A, typename B> void f(A a, B b) {
    A tmp = a; // 模版参数列表的A隐藏模版外的A，此处tmp类型为A，而不是double
    double B; // 不可重用模版参数B
    }
    ```
    3. 一个模版参数在一个模版参数列表中只能出现一次，不能重复
    `template <typename V, typename V>// 错误`

* 模版声明的参数
    * 模版声明必须有模版参数列表
    * 多个模版声明以及对应的模版定义中的参数可以都不同名，但是参数类型和数量必须相同

* 使用模版参数的类型成员
    * 关键字typename显式告诉编译器这是个类型，而不是变量名字
    ```C++
    template <typename T>
    typename T::value_type top(const T& c) { //告诉编译器T::value_type是一个类型，而不是名字
        if (!c.empty())
            return c.back();
        else
            return typename T::value_type();
    }
    ```

* 默认的模版参数：可以为函数提供默认实参，也可以为模版参数提供默认实参
    * 一个参数 右侧所有参数都有默认实参的时候 才可以有默认参数
    * 实例化类模版必须要有<模版实参>，但是当所有模版形参都有默认参数时，<>可以为空


### 成员模版
成员模版：一个类，无论是类模版还是普通类，都可以包含一个 本身是模版的成员函数

* 普通类的成员模版：成员模版的类型由编译器推断

* 类模版的成员模版：类模版和成员模版各自有自己的模版参数
    * 在类模版外定义成员模版时：
        * 同时为类模版和成员模版提供模版参数列表
        * 类模版参数列表在前，成员模版参数在后
    * 实例化类模版的成员模版：对象调用成员模版函数
        * 根据对象类型推断类模版的实参
        * 根据调用函数的函数实参实例化成员模版


### 控制实例化
* 实例化带来的问题
    * 模版通常保存在头文件中
    * 头文件被多个源文件包含进去
    * 当多个源文件使用相同的模版，并且提供相同的模版实参，那么每个源文件都有相同的模版实例
    * 带来的额外开销可能非常严重

* 解决办法：
    * 声明extern关键字：`extern template class Blob<string>;`
        * 表示不会在本文件中生成实例化代码
        * 在其他文件中生成了实例化代码，可以重复使用
    * 前提是：在其他文件必须有显式实例化的定义`template int compare(const int&, const int&);`

* 显式实例化
    * 会实例类模版的所有成员
    * 即使我们不使用某些成员，它们也会被实例化。
        * 因此：使用某个类型A显式实例化类模版，类模版中的所有操作对于类型A都必须是可以执行的

* 隐式实例化（编译器推断）：模版成员操作被用到才进行实例化


### 效率和灵活性 ？？？待完成
-------------------------


## 模版实参推断
> 从函数实参来确定模版实参的过程叫模版实参推断

### 推断过程中涉及的类型转换
* 面对新类型的函数实参，通常来说编译器的做法是生成实例化类模版生成新的实例
* 对少数几种类型的函数实参才进行类型转换
    * 将非const对象的引用或指针 ——> const引用/指针形参
    * 如果函数形参不是引用类型
        * 数组实参--->指向首元素指针
        * 函数实参--->该函数类型的指针

* 模版函数参数类型不是模版参数类型，那么就可以对实参进行正常的类型转换
* 多个函数形参使用相同模版参数类型，那么传递给这些形参的实参类型必须相同


### 显式指定实参*优先于*编译器推断 
* 指定的显式模版实参 放在<>中
    * 第一个模板实参与模版参数列表中第一个模板参数匹配，以此类推。
    * 右边的参数可以忽略，前提是他们可以被编译器从函数实参推断出来
* 如果对函数在<>中显式指定实参,那么函数实参可以进行正常的类型转换

### 推断尾置返回类型
* 当我们希望用户指定返回类型时，我们可以通过在函数<>中指定显式实参
* 但是要求用户操作会给用户添加额外负担
* 解决办法：使用尾置返回类型
```C++
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    return *beg; 
}
```
* ~~进行类型转换的标准库模版类~~ 
    * <type_trait>头文件中
    * 常用于模版元编程程序设计
    * 例如：remove_reference
        * 是一个有模版参数列表的类模版
        * 有一个名为type的public成员
            * type和模版参数类型相关
                * 如果不能转换模版参数，那么type就是模版参数类型


### 通过函数指针类型*推断*函数模版实参
```C++
template <typename T> int compare(const T&, const T&);
// 我们从pf1函数指定变量推断compare函数模版的实参，从而实例化它
int (*pf1)(const int&, const int&) = compare;
```

### 当函数参数是引用时进行推断

* 函数参数是左值引用时
    * 函数参数类型是普通的左值引用T&时 `template <typename T> void f1(T&);`
        * 函数调用只能使用左值
            * `f1(i);` i是个int，则推断模版参数类型T为int
            * `f1(ci);` ci是个const int，则模版参数T推断为const int
            * `f1(5);`错误，传递的必须是左值
    * 函数参数类型是const左值引用T&时`template <typename T> void f2(const T&);`
        * T的类型推断不会是const类型，因为const已经是f2参数的一部分了，所以不会是T的一部分
        * 函数调用可以接受一个右值并且能够正常进行推断
            * `f2(i);` i是个int，则推断模版参数类型T为int
            * `f2(ci);` ci是个const int，模版参数T推断为int
            * `f2(5);` const& 可以绑定到右值，模版参数T推断为int

* 函数参数时右值引用时`template <typename T> void f3(T&&);`:
    * 可以传递一个右值，T的类型推断为右值实参的类型
        * `f3(43);`则模版参数T推断为int
    * 2个例外规则：
        1. 通常情况：不能将右值引用绑定到左值。
            * 例外规则允许将T&&绑定到左值时（T是模版类型参数），编译器会判断T为实参的左值引用类型
            * 例子：
                * `f3(i);` 如果i是个int，则模版参数T被推断为int&
                * `f3(ci);`如果ci是个const int,则模版参数T被推断为const int&
            * T被推断成int&让函数f3的函数参数看起来是一个int&的右值引用,引用折叠后为int&
        2. 通常情况：我们不能直接定义一个引用的引用，
            * 例外情况：但是通过模版类型参数间接定义是可以的。在这种情况下，创建引用的引用，这些引用形成了折叠
            * 给定类型X
                * X& &——左值引用的左值引用，折叠成X&
                * X& &&——左值引用的右值引用，折叠成X&
                * X&& &——右值引用的左值引用，折叠成X&
                * X&& &&——右值引用的右值引用，折叠成X&&（特殊情况）
        * 以上两个例外导致：将左值传递给函数的右值引用参数，编译器推断模版类型参数为实参的左值引用，根据折叠规则T&&折叠为左值引用类型

    * 重要结果:如果实参是一个左值i(int)，那么推断出来的T为左值引用(int&)，函数模版会被实例化成`void f3<int&>(int&);` 
    * 延伸：如果函数模版的一个函数参数是指向模版参数类型的右值引用T&&，则可以传递给他任意类型的实参数。包括左值和右值

    * 编写接受右值引用的模版函数的两种用途
        * 实参转发
        * 模版重载


### 理解std::move:使用右值引用模版最好的例子
```C++
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}
```
* move是一个函数模版
* move的参数是一个指向模版类型参数的右值引用，
* 它既可以接受左值，又可以接受右值（推断出类型为被引用的类型）
* 使用move可以获得一个绑定到左值上的右值引用
* 具体实现
    * 使用了标准库的类型转换模版类remove_refernece<T>::type
    * 内层通过static_cast实现
        * stati_cast常规使用：用于合法的类型转换
        * 针对右值的特殊规定：可以使用static_cast显式将左值转成右值引用
            * 能够通过将右值引用绑定到左值能过截断左值
            * 通过使用static_cast进行强制转换，阻止其他形式意外进行的转换
    * 当使用左值作为move实参，move函数模版实例化为T&& move(T&)
    * 当使用右值作为move实参，move函数模版实例化为T&& move(T&&)


### 转发
> 将函数参数定义为指向模版参数的右值引用，可以保持实参的所有类型信息。
* 保留的类型信息
    * 对应的const属性
    * 左值
    * 右值

* std::forward<Type>(arg)
    * 定义在utility里面
    * 必须通过显式模版实参来调用
    * 返回该显式实参类型的右值引用。例如std::forward<T>的返回类型是T&&
    * 它能过保持原始实参的类型：所以就像是纹丝不动把它转发一样。

```C++
// arg是模版类型参数的右值引用
// 实参传递给arg，那么type将保留实参的所有类型信息
// 1. 如果实参是一个右值，那么Type是一个普通类型，并且不是引用。forward返回Type&&，是个右值
// 2. 如果实参是一个左值，通过引用折叠，那么Type是一个左值引用类型。在此情况下，forward返回左值引用的右值引用，再次引用折叠，得到左值引用类型，最后返回左值。
// 3. 同样，作为类型信息的const属性，通过forward也能得到保持
template <typename Type> intermediary(Type &&arg) 
{
    finalFcn(std::forward<Type>(arg));
}
```

-------------
## 重载和模版

* 函数模版的重载
    * 函数模版可以被另外一个函数模版或者普通非模版函数重载，前提是函数名相同，且具有不同数量或者类型的参数

* 函数模版的匹配规则：
    1. 对于一个函数调用，它的候选函数包括 所有根据函数调用进行模版实参推断成功的 函数模版实例
    2. 推断成功代表：后续的函数模版总是可行的，模版推断会排除不可行的函数模版
    3. ~~与往常一样，所有可行函数按类型转换排序~~????
    4. 优先选择最优匹配函数
    5. 没有最优匹配函数，则在同等匹配项中优先选择非模版函数
    6. 如果同等匹配函数选项没有非模版函数，而是有多个模版函数，优先选择更特例化的函数模版
    7. 否则调用有歧义


## 可变参数模版

可变参数模版
: 接受可变参数数目的的模版函数或者模版类

* 可变数目参数的被称为参数包
* 两种参数包：
    1. 模版参数包：0个或多个模版参数
    2. 函数参数包：0个或多个函数参数
* 我们用一个省略号指出一个模版参数或者函数参数是一个包
```C++
// Args 表示0个或多个模版类型参数，是一个模版参数包
// rest 表示0个或多个函数参数，是一个函数参数包
template <typename T, typename... Args>
void foo(const T &t, const Args& ... rest);
```
* sizeof... 运算符用来知道模版参数包和函数参数包中有多少个元素
```C++
template<typename ... Args> 
void g(Args ... args) {
    cout << sizeof...(Args) << endl; 
    cout << sizeof...(args) << endl; 
}
```

### 编写可变参数模版
* 可变参数模版通常是递归的
* 非可变参数模版比可变参数模版更特例化
```C++
template< typename T>
ostream &print( ostream &os,const T &t)
{
    return os << t; //将会打印包中最好一个参数
}
template< typename T,typename... Args>
ostream &print( ostream &os,const T &t,const Args&... rest)
{
    os << t << ","; // 打印第一个实参
    return print(os,rest...); // 递归调用，打印其他实参
}
```
### 包的扩展
* 对于包（模版参数包/函数参数包），除了获取它们元素个数，唯一能做的事情是扩展包
* **扩展包：** 分解为构成的元素，并且对每个元素应用模式，获得扩展后的列表
    * 首先提供用于扩展的模式
    * 通过在模式右边放置省略号`...`触发扩展操作
```C++
template <typename T, typename... Args>
ostream &
print(ostream &os, const T &t, const Args&... rest) // 扩展模版参数包，为print生成函数列表。编译器将模式const Arg&应用到Args每个元素中，扩展结果是 const type1&，const type2&，const type3&
{
    os << t << ", ";
    return print(os, rest...); // 扩展函数参数包，为print调用生成实参列表。模式是函数参数包中的名字rest，扩展结果是rest包中元素组成的、逗号分隔的列表
}
```
* 理解包的扩展：C++y语言允许更复杂的扩展模式，扩展的模式会被独立应用于包的每个元素
```C++
template <typename... Args>
ostream &errorMsg(ostream &os, const Args&... rest)
{
    // 相当于print(os,debug_rep(a1),debug_rep(a2),...,debug_rep(an)
    // a1/a2/a3都是包中的元素
    return print(os, debug_rep(rest)...);
}
```


### 转发参数包
* 转发参数包：结合std::forward和可变参数模版来编写函数
* 可变函数参数通常将它们的参数转发给其他的函数
```C++
// fun函数模版有多个函数参数，每个函数参数都是模版参数类型的右值引用
// 由于fun函数模版的函数参数是右值引用，因此我们可以传递给它任意类型的实参
// 由于我们使用std::forward(),实参的所有类型信息在调用work时得到保留
template<typename... Args>
voidfun(Args&&...args)  //将模版参数包Args扩展成右值引用的列表
{
    // 这里既扩展模版参数包Args，也扩展函数参数包args
    work(std::forward<Args>(args)...);
}
```

## 模版特例化

* Question:我们为什么需要模版特例化
* Answer:
    1. 编写单一的模版，让它对任何类型的实参能够实例化，太过于理想，并不能总是办到。如果通用模版定义对特定的类型不合适导致编译失败
    2. 我们希望利用特殊的知识编写更加高效的代码，而不是一个模版通用化 

* 函数模版中的特例化
    * 为了指出我们在实例化一个模版，使用关键字template<>
    * <> 指出我们将为原模版的每一个模版参数提供实参
    * 特例化函数的参数必须与一个先前声明的模版中对于的类型匹配
    * 特例化一个模版，原模版的声明必须在作用域内
    * 在任何使用模版实例的代码之前，特例化版本的声明也必须存在作用域中
        * 如果丢失了特例化版本的声明，编译器可以用原模版生成新的实例
        * **模版及其特例化版本应该放在同一个头文件中，所有同名模版的声明应该放在前面，后面是这些模版的特例**

* 特例化和函数重载
    * 手动定义函数模版的特例时，是我们人为接管了编译器的工作
    * 函数模版的特例化实际上是一个实例，而不是函数名的一个重载版本
    * 特例化一般不影响函数的重载，但是一个函数定义成特例化版本还是独立的非模版函数，会影响函数的匹配

* 类模版中的特例化
    * [类模版全部特例化](../代码/chapter16/full-specialization/full.hpp)
        * 关键字template<> 表示类模版特例化
        * 为所有模版参数提供实参
    * 类模版部分特例化：
        * 类别
            * [为部分而非全部模版参数提供实参](../代码/chapter16/partial-specialization1/partial.hpp)
            * [指定模版参数的一部分特性，而不是全部特性](../代码/chapter16/partial-specialization2/partial.hpp)
        * 部分特例化的实例本身是一个模版:因为还有部分模版参数的存在
        * 部分特例化的名字与原模版的名字相同
        * 未完全确定的模版参数，在部分特例化的模版参数列表中有一项与之对应
        * 在部分特例化的类名之后，要为特例化的模版参数指定实参，位于模版名后的<>中
    * 类模版成员函数的特例化：如特例化Foo<int>:bar()成员