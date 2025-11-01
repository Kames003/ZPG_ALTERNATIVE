#pragma once

#include <vector>
#include <cstdio>
#include <cstdlib>

class DrawableObject;

/**
 * Manager pre správu všetkých vykresľovaných objektov
 *
 * UPRAVENÉ pre Cvičenie 7:
 * - Odstránená funkcionalita Skybox
 * - Odstránená podpora textúr
 * - Jednoduchšie vykresľovanie
 */
class ObjectManager
{
private:
    std::vector<DrawableObject*> objects;  // Všetky objekty v scéne

public:
    /**
     * Pridá objekt do scény
     */
    void addDrawableObject(DrawableObject* d);

    /**
     * Vykreslí všetky objekty (bez textúr)
     */
    void drawObjects();

    /**
     * Nastaví viditeľnosť objektu (pre budúce rozšírenia)
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