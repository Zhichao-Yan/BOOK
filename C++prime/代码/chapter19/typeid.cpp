#include <iostream>

using namespace std;
 
int func(int a,int b)
{
    return a + b;
}

class A{
private:
    int a;
    int b;
public:
    void Print() { cout<<"This is class A."<<endl; }
};
struct B{
    int c;
    double d;
};

int main()
{
    // 内置类型
    short s = 2;
    cout<<typeid(s).name()<<endl;
    int a = 10;
    cout<<typeid(a).name()<<endl;
    long b = 10l;
    cout<<typeid(b).name()<<endl;
    long long c = 10ll;
    cout<<typeid(c).name()<<endl;

    unsigned int a1 = 11u;
    cout<<typeid(a1).name()<<endl;
    unsigned long b1 = 11ul;
    cout<<typeid(b1).name()<<endl;
    unsigned long long c1 = 11ull;
    cout<<typeid(c1).name()<<endl;

    bool d = true;
    cout<<typeid(d).name()<<endl;
    char e = 'e';
    cout<<typeid(e).name()<<endl;
    wchar_t e1 = L'e';
    cout<<typeid(e1).name()<<endl;
    char16_t e2 = u'颜';
    cout<<typeid(e2).name()<<endl;
    char32_t e3 = U'超';
    cout<<typeid(e3).name()<<endl;

    float f = 1.0f;
    cout<<typeid(f).name()<<endl;
    double g = 3.1415926;
    cout<<typeid(g).name()<<endl;
    long double h = 3.1415926l;
    cout<<typeid(h).name()<<endl;

    int array[10];//array
    cout<<typeid(array).name()<<endl; // 数组
    int func(int a,int b);//function
    cout<<typeid(func).name()<<endl; // 函数

    const int ci = 42;
    cout<<typeid(ci).name()<<endl; // 忽略顶层const属性
    const int &r = ci;
    cout<<typeid(r).name()<<endl; // 返回引用对象的类型

    A *C = new A;
    A object;
    cout<<typeid(A).name()<<endl; // 可以接受类型
    cout<<typeid(object).name()<<endl; // 可以接受表达式。例如对象object
    cout<<typeid(C).name()<<endl; // 可以是表达式。例如指针变量
    cout<<typeid(B).name()<<endl; // 返回结构体的类型
    
    return 0;
}