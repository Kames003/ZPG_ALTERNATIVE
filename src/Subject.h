#pragma once

#include <vector>
#include "Observer.h"

class Subject
{
protected:
    std::vector<Observer*> observerCollection;
    virtual void notifyAll(int message) = 0;
public:
    void attach(Observer* o);
};