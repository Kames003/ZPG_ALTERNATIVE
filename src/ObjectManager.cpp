#include "ObjectManager.h"
#include "DrawableObject.h"

void ObjectManager::addDrawableObject(DrawableObject* d)
{
    if (d) {
        objects.push_back(d);
    }
}

void ObjectManager::addSkybox(DrawableObject* d)
{
    skybox = d;
}

void ObjectManager::drawSkybox()
{
    if (!skybox) return;
    
    // Skybox sa renderuje s depth testom, ale bez zápisu do depth bufferu
    glDepthMask(GL_FALSE);
    
    // Bindni cubemap textúru
    if (skybox->textures.size() > 0) {
        skybox->updateTexture("tex", skybox->textures[0]->position);
    }
    
    skybox->updateModelMatrix();
    skybox->drawModel();
    
    glDepthMask(GL_TRUE);
}

void ObjectManager::drawObjects()
{
    // Vykresli všetky objekty (s podporou textúr)
    for (DrawableObject* obj : objects)
    {
        if (!obj || !obj->visible) continue;
        
        // Podľa počtu textúr rozlišujeme rendering
        switch (obj->textures.size())
        {
            case 0:
                // Bez textúr - len farba
                obj->updateModelMatrix();
                obj->drawModel();
                break;
                
            case 1:
                // Jedna textúra (diffuse/albedo)
                obj->updateTexture("tex", obj->textures[0]->position);
                obj->updateModelMatrix();
                obj->drawModel();
                break;
                
            case 2:
                // Dve textúry (albedo + normal map)
                obj->updateTexture("tex", obj->textures[0]->position);
                obj->updateTexture("normalMap", obj->textures[1]->position);
                obj->updateModelMatrix();
                obj->drawModel();
                break;
                
            default:
                // Viac textúr - zatiaľ len prvú použijeme
                obj->updateTexture("tex", obj->textures[0]->position);
                obj->updateModelMatrix();
                obj->drawModel();
                break;
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