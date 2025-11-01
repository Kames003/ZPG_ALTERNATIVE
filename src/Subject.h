#pragma once

#include <vector>
#include "Observer.h"

class Subject
{
protected:
    std::vector<Observer*> observers;
    virtual void notify(int message) = 0;
public:
    void attach(Observer* o);
};