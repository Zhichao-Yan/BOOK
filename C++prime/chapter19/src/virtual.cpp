#include <iostream>
using namespace std;

class Person
{
protected:
    string name;
public:
    Person(string name=""):name(name){};
    virtual ~Person(){}  
    virtual string getInfo(){return name;}
};

class Student: public Person
{
    string studentid;
public:
    Student(string name="", string sid=""):Person(name),studentid(sid){};
    string getInfo(){return name+":("+studentid + ")";}
};
int main()
{
    Person person("Yu");
    Student student("Sam", "20210212");
    Person* pp = &student;
    cout << "pp->getInfo():" << pp->getInfo() << endl;
    pp = &person;
    cout << "pp->getInfo():" << pp->getInfo() << endl;
    Person& rp1 = student;
    cout << "rp1->getInfo():" << rp1.getInfo() << endl;
    Person& rp2 = person;
    cout << "rp2->getInfo():" << rp2.getInfo() << endl;
    return 0;
}