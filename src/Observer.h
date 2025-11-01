#pragma once

class Observer
{
public:
    virtual void update(int message) = 0;
};