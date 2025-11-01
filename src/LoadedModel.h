#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "AbstractModel.h"

// Model načítaný zo súboru (OBJ, FBX, atď.) pomocou knižnice ASSIMP
// Podporuje normály, textúrové koordináty
class LoadedModel : public AbstractModel
{
protected:
    int count;  // Počet vrcholov na vykreslenie

public:
    // Konštruktor - načíta model zo súboru
    // Podporované formáty: .obj, .fbx, .dae, .3ds, atď.
    LoadedModel(const char* path);
    
    void draw() override;
};