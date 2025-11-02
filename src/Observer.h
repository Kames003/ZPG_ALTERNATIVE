#pragma once

class Observer
{
public:
    virtual void notify(int message) = 0;
};