#include <iostream>
#include <string>

using namespace std;


class Screen{
    friend class Window_mgr;
public:
    typedef string::size_type pos;
    Screen() = default;
    Screen(pos ht,pos wd,char c):height(ht),width(wd),contents(ht * wd,c){}
    char get() const { return contents[cursor]; }
    inline char get(pos ht,pos wd) const;
    Screen& move(pos r,pos c);
    void some_member() const;
    Screen& set(char);
    Screen& set(pos,pos,char);
    Screen& display(ostream &os)
    {
        do_display(os);
        return *this;
    }
    const Screen& display(ostream &os) const
    {
        do_display(os);
        return *this;
    }
private:
    pos cursor = 0;
    pos height = 0,width = 0;
    string contents;
    mutable size_t access_ctr;
    void do_display(ostream &os) const { os << contents; }
};

inline 
Screen& Screen::set(char c)
{
    contents[cursor] = c;
    return *this;
}

inline
Screen& Screen::set(pos r,pos col,char c)
{
    contents[r * width + col] = c;
    return *this;
}

// 内联的成员函数必须放在头文件中和普通的内联函数一样
inline
Screen& Screen::move(pos r,pos c)
{
    pos row = r * width;
    cursor = row + c;
    return *this;
}
char Screen::get(pos r,pos c) const
{
    pos row = r * width;
    return contents[row + c];
}