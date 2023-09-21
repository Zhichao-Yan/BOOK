#ifndef BIRD_H
#define BIRD_H
#include "Flyable.hpp"

class Bird:public Flyable
{
public:
    void foraging();
    void takeoff();
    void land();
};

#endif