#include <string>
using std::string;
using std::size_t;

class HasPtr{
private:
    string *ps;
    size_t *use;
    int i;
public:
    HasPtr(const string &s = string()):ps(new string(s)),use(new size_t(1)),i(0){}
    HasPtr(const HasPtr &p):ps(p.ps),use(p.use),i(p.i) { ++*use; }
    ~HasPtr()
    {
        if(--*use == 0)
        {
            delete use;
            delete ps;
        }
    }
    HasPtr& operator=(const HasPtr &p);
};
HasPtr& HasPtr::operator=(const HasPtr &p)
{
    ++(*(p.use));
    if(--(*use) == 0)
    {
        delete use;
        delete ps;
    }
    ps = p.ps;
    use = p.use;
    i = p.i;
    return *this;
}