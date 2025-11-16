#pragma once

#include <vector>
#include <cstdio>
#include <cstdlib>

class DrawableObject;

class ObjectManager
{
private:
    std::vector<DrawableObject*> objects;
    DrawableObject* skybox;

public:

    void addDrawableObject(DrawableObject* d);
    void addSkybox(DrawableObject* d);
    void drawSkybox();
    void drawObjects();
    void setObjectVisibility(int index, bool visible);
    void removeDrawableObject(int index);
    DrawableObject* getDrawableObject(int index);
    int getObjectCount() const { return static_cast<int>(objects.size()); }
};