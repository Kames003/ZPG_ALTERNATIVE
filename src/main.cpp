#include "Application.h"
#include <cstdlib>

int main()
{
    Application* a = new Application();
    a->initScene();
    a->run();

}