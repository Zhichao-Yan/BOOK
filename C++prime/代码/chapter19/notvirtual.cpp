#include <iostream>
using namespace std;

class Person
{
protected:
    string name;
public:
    Person(string name=""):name(name){};
    virtual ~Person(){}  
    string getInfo(){return name;}
};

class Student: public Person
{
    string studentid;
public:
    Student(string name="", string sid=""):Person(name),studentid(sid){};
    string getInfo(){return name+":("+studentid + ")";}
    string show(){ return "I'm a student";}
};

int main()
{
    Person person("Yu");
    Student student("Sam", "20210212");
    Person& rp = student; //基类引用绑定派生类对象
    Person* pp = &student; //基类指针指向派生类对象

    // 在子类中被覆盖，只会调用父类的函数
    cout << "rp.getInfo():" <<rp.getInfo() << endl;
    cout << "pp->getInfo():" <<pp->getInfo() << endl;

    //show()函数只存在于子类中，调用失败
    //cout << "pp->show():" <<pp->show() << endl; 
    return 0;
}