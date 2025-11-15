#pragma once

#include <vector>
#include <cstdio>
#include <cstdlib>

class DrawableObject;

class ObjectManager
{
private:
    std::vector<DrawableObject*> objects;  // Všetky objekty v scéne
    DrawableObject* skybox;                // Skybox (renderuje sa osobitne)

public:
    /**
     * Pridá objekt do scény
     */
    void addDrawableObject(DrawableObject* d);
    
    /**
     * Nastaví skybox (renderuje sa pred všetkým ostatným)
     */
    void addSkybox(DrawableObject* d);

    /**
     * Vykreslí skybox (volaj PRVÝ pred drawObjects)
     */
    void drawSkybox();
    
    /**
     * Vykreslí všetky objekty (s podporou textúr)
     */
    void drawObjects();

    /**
     * Nastaví viditeľnosť objektu (pre budúce rozšírenie vraj to ma byt next week darček / zombos
     */
    void setObjectVisibility(int index, bool visible);

    /**
     * Získa konkrétny objekt podľa indexu
     */
    DrawableObject* getDrawableObject(int index);

    /**
     * Počet objektov v scéne
     */
    int getObjectCount() const { return static_cast<int>(objects.size()); }
};