#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "AbstractModel.h"

// Model načítaný zo súboru (OBJ, FBX, atď.) pomocou knižnice ASSIMP

class LoadedModel : public AbstractModel
{
protected:
    int count;  // Počet vrcholov na vykreslenie

public:
    // Konštruktor - načíta model zo súboru
    LoadedModel(const char* path);
    
    void draw() override;
};