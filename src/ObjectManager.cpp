#include "ObjectManager.h"
#include "DrawableObject.h"

void ObjectManager::addDrawableObject(DrawableObject* d)
{
    if (d) {
        objects.push_back(d);
    }
}

void ObjectManager::drawObjects()
{
    // Vykresli všetky objekty (bez textúr, bez skybox)
    for (DrawableObject* obj : objects)
    {
        if (obj) {
            obj->drawModel();
        }
    }
}

void ObjectManager::setObjectVisibility(int index, bool visible)
{
    if (index >= 0 && index < static_cast<int>(objects.size()))
    {
        // Budúce rozšírenie - zatiaľ len placeholder
        // objects[index]->setVisible(visible);
    }
}

DrawableObject* ObjectManager::getDrawableObject(int index)
{
    if (index < 0 || index >= static_cast<int>(objects.size()))
    {
        printf("ERROR: DrawableObject index %d out of range (0-%d)\n",
               index, static_cast<int>(objects.size()) - 1);
        return nullptr;
    }

    return objects[index];
}