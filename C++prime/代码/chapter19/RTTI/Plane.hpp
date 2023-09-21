#ifndef PLANE_H
#define PLANE_H
#include "Flyable.hpp"

class Plane:public Flyable
{
public:
    void transport();
    void takeoff();
    void land();
};


#endif