#include <string>
using std::string;

class HasPtr{
private:
    string *ps;
    int i;
public:
    HasPtr(const string &s = string()):ps(new string(s)),i(0){}
    HasPtr(const HasPtr &p):ps(new string(*p.ps)),i(p.i){}
    HasPtr(HasPtr &&p) noexcept;
    HasPtr& operator=(const HasPtr &p);
    HasPtr& operator=(HasPtr p); // 处理了自赋值情况且天然就是异常安全的
    ~HasPtr(){ delete ps; }
    friend void swap(HasPtr&,HasPtr&);
};
inline
void swap(HasPtr &lhs,HasPtr &rhs)
{
    using std::swap;
    swap(rhs.ps,lhs.ps);
    swap(rhs.i,lhs.i);
    return;
}

HasPtr& HasPtr::operator=(const HasPtr &p)
{
    string * newp = new string(*p.ps);
    delete ps;
    ps = newp;
    i = p.i;
    return *this;
}
HasPtr& HasPtr::operator=(HasPtr p)
{
    swap(*this,p);
    return *this;
}

HasPtr::HasPtr(HasPtr &&p) noexcept
:ps(p.ps),i(p.i)
{
    p.ps = nullptr;
}