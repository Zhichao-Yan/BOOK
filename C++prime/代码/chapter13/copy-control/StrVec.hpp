#ifndef STRVEC_H
#define STRVEC_H
#include <memory>
#include <string>
#include <utility>

using std::allocator;
using std::string;
using std::pair;
using std::uninitialized_copy;
using std::make_move_iterator;


class StrVec{
public:
    StrVec():elements(nullptr),first_free(nullptr),cap(nullptr){}
    StrVec(const StrVec&);
    StrVec(StrVec &&s) noexcept
    :elements(s.elements),first_free(s.first_free),cap(s.cap)
    {
        s.elements = nullptr;
        s.first_free = nullptr;
        s.cap = nullptr;
    }
    StrVec& operator=(StrVec &&rhs) noexcept
    {
        if(this != &rhs)
        {
            free();
            elements = rhs.elements;
            first_free = rhs.first_free;
            cap = rhs.cap;
            rhs.elements = rhs.first_free = rhs.cap = nullptr;
        }
        return *this;
    }
    StrVec& operator=(const StrVec &rhs);
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    string* begin() const { return elements; }
    string* end() const { return first_free; }
    void push_back(const string&);
    void push_back(string &&);
private:
    static allocator<string> alloc;
    void chk_n_alloc()
    {
        if(size() == capacity())
            reallocate();
    }
    pair<string*,string*> alloc_n_copy(const string*,const string*);
    void free();
    void reallocate();
    void reallocate_move();
    string *elements;
    string *first_free;
    string *cap;
};


StrVec::StrVec(const StrVec &s)
{
    auto newaddr = alloc_n_copy(s.elements,s.first_free);
    elements = newaddr.first;
    first_free = newaddr.second;
    cap = first_free;
}
StrVec::~StrVec()
{
    free();
}
StrVec& StrVec::operator=(const StrVec &rhs)
{
    auto data = alloc_n_copy(rhs.begin(),rhs.end());
    free();
    elements = data.first;
    first_free = data.second;
    cap = first_free;
    return *this;
}
void StrVec::push_back(const string &s)
{
    chk_n_alloc();
    alloc.construct(first_free++,s);
}

pair<string*,string*> 
StrVec::alloc_n_copy(const string *b,const string *e)
{
    auto data = alloc.allocate(e - b);
    return {data,uninitialized_copy(b,e,data)};
}
void StrVec::free()
{
    if(elements)
    {
        for(auto p = first_free;p != elements;)
        {
            alloc.destroy(--p);
        }
        alloc.deallocate(elements,cap - elements);
    }
}
void StrVec::reallocate()
{
    auto newcapacity = size()? 2 * size():1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto src = elements;
    for(size_t i = 0; i != size(); ++i)
    {
        alloc.construct(dest++,std::move(*src++));
    }
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;

}
void StrVec::reallocate_move()
{
    auto newcapacity = size()? 2 * size():1;
    auto first = alloc.allocate(newcapacity);
    auto last = uninitialized_copy(make_move_iterator(begin()),make_move_iterator(end()),first);
    free();
    elements = first;
    first_free = last;
    cap = elements + newcapacity;

}
void StrVec::push_back(string &&s)
{
    chk_n_alloc();
    alloc.construct(first_free++,std::move(s));
    return;
}
#endif