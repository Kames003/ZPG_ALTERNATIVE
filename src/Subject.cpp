#include "Subject.h"

void Subject::attach(Observer* o)
{
    observers.push_back(o);
}
