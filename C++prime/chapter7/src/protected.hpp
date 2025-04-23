class Base{
protected:
    int k;
};
class Sneaky:public Base{
    friend void clobber(Sneaky&);
    friend void clobber(Base&);   
    int j;
};
void clobber(Sneaky &s)
{
    s.j = 0;
    s.k = 0; // 派生类的友元可以通过派生类对象访问protected成员，正确
}
void clobber(Base &b)
{
    b.k = 0;// 派生类的友元不能通过基类对象访问protected成员，错误
}