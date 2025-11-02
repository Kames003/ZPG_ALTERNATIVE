#include "Subject.h"

void Subject::attach(Observer* o)
{
    observerCollection.push_back(o);
}
