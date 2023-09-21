#include <iostream>
using namespace std;

class Person
{
protected:
    string name;
public:
    Person(string name=""):name(name){};
    ~Person(){}  
    string getInfo(){return name;}
};

class Student: public Person
{
private:
    string studentid;
public:
    Student(string name="", string sid=""):Person(name),studentid(sid){};
    string getInfo(){
        return name+":("+studentid + ")";
    }
    string show(){
        return "I'm a student";
    }
};

int main()
{
    // 多态版本
    Person  * person = new Person("Yan");
    Student * ps = (Student*)person; // danger! 显式强制转换
    cout << "person->getInfo():" << person->getInfo() << endl;
    // ps指针变量保存的是父类Person对象的内存地址
    // ps调用getInfo函数会访问Student类的private成员studentid
    // Person对象本身不包含Studentid成员，访问派生类私有成员非常危险
    cout << "ps->getInfo():" << ps->getInfo() << endl; // danger if getInfo is not virtual
    cout << "ps->show():" << ps->show() << endl;
    delete person;

    // 非多态版本
    // Person  person("Yan");
    // Student * ps = (Student*)&person; // danger! 显式强制转换
    // cout << "person.getInfo():" << person.getInfo() << endl;
    // // ps指针变量保存的是父类Person对象的内存地址
    // // ps调用getInfo函数会访问Student类的private成员studentid
    // // Person对象本身不包含Studentid成员，访问派生类私有成员非常危险
    // cout << "ps->getInfo():" << ps->getInfo() << endl; // danger if getInfo is not virtual
    // cout << "ps->show():" << ps->show() << endl;
    return 0;
}
