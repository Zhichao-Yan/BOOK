#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <set>
#include "Folder.hpp"
using std::string;
using std::set;

class Folder;
class Message{
    friend class Folder;
    friend void swap(Message &lhs,Message &rhs);
public:
    explicit Message(const string &str = ""):contents(str){}
    Message(const Message&);
    ~Message();
    Message& operator=(const Message&);
    Message(Message &&m):contents(std::move(m.contents))//执行string的移动构造函数
    {
        move_folders(&m);
    }
    Message& operator=(Message &&rhs)
    {
        if(this != &rhs)
        {
            remove_to_folders();
            contents = std::move(rhs.contents);
            move_folders(&rhs);
        }
        return *this;
    }
    void remove(Folder&);
    void save(Folder&);
    void move_folders(Message *m);
private:
    string contents;
    set<Folder*> folders;
    void add_to_folders(const Message&);
    void remove_to_folders();
};


void Message::save(Folder &f )
{
    folders.insert(&f);
    f.addMsg(this);
}
void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.remMsg(this);
}
void Message::add_to_folders(const Message &m)
{
    for(auto f:m.folders)
        f->addMsg(this);
}
Message::Message(const Message &m):contents(m.contents),folders(m.folders)
{   
    add_to_folders(m);
}
void Message::remove_to_folders()
{
    for(auto f:folders)
        f->remMsg(this);
}
Message::~Message()
{
    remove_to_folders();
}

Message& Message::operator=(const Message &rhs)
{
    remove_to_folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_folders(rhs);
    return *this;
}

void swap(Message &lhs,Message &rhs)
{
    using std::swap;
    for(auto f:lhs.folders)
        f->remMsg(&lhs);
    for(auto f:rhs.folders)
        f->remMsg(&rhs);
    swap(lhs.folders,rhs.folders);
    swap(lhs.contents,rhs.contents);
    for(auto f:lhs.folders)
        f->addMsg(&lhs);
    for(auto f:rhs.folders)
        f->addMsg(&rhs);
    return;
}
void Message::move_folders(Message *m)
{
    folders = std::move(m->folders);
    for(auto f:folders)
    {
        f->remMsg(m);
        f->addMsg(this);
    }
    m->folders.clear();
}


#endif