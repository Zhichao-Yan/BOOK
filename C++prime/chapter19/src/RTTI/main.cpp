#include <iostream>
#include <typeinfo>
#include "Flyable.hpp"
#include "Bird.hpp"
#include "Plane.hpp"


void execute(Flyable *f)
{
    std::cout <<typeid(*f).name() << std::endl;
    f->takeoff();
    if(typeid(*f) == typeid(Bird))
    {
        Bird *ptr = dynamic_cast<Bird*>(f);
        ptr->foraging();
    }
    if(typeid(*f) == typeid(Plane))
    {
        Plane *ptr = dynamic_cast<Plane*>(f);
        ptr->transport();
    }
    f->land();
    return;
}

int main()
{
    Bird d;
    Plane p;
    execute(&d);
    execute(&p);
    return 0;
}

