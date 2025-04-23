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
private:
    string studentid;
public:
    Student(string name="", string sid=""):Person(name),studentid(sid){};
    string getInfo(){
        return name+":("+studentid + ")";
    }
};

int main()
{
    Person person("Yan");
    Student student ("Sam", "20210212");
    // Student * ps = (Student*)&person; // danger! 显式强制转换，转换成功
    // waring：'dynamic_cast<class Student*>(Person person)' can never succeed
    Student *ps = dynamic_cast<Student*>(&person); // 上面的强制转换成功，但是这里转换失败
    printf("address = %p\n", ps);//返回nullptr指针=0
    Person * pp = dynamic_cast<Person*>(&student);
    printf("address = %p\n", pp);
    return 0;
}
 

