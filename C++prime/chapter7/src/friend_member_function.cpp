#include <iostream>
#include <string>

using namespace std;


class Window_mgr{
public:
    using ScreenIndex = vector<Screen>::size_type;
    void clear(ScreenIndex i);
private:
    vector<Screen> screens{Screen(24,80,' ')};
};


class Screen{
    friend void Window_mgr::clear(ScreenIndex i);
public:
    typedef string::size_type pos;
    Screen() = default;
    Screen(pos ht,pos wd,char c):height(ht),width(wd),contents(ht * wd,c){}
    char get() const { return contents[cursor]; }
    inline char get(pos ht,pos wd) const;
    Screen& move(pos r,pos c);
private:
    pos cursor = 0;
    pos height = 0,width = 0;
    string contents;
};

void Window_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];
    s.contents = string(s.height * s.width,'1');
}

int main()
{
    Window_mgr w;
    w.clear(0);
}