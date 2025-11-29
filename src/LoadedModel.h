#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "AbstractModel.h"


class LoadedModel : public AbstractModel
{
protected:
    int count;  // Počet vrcholov na vykreslenie

public:
    // Konštruktor - načíta model zo súboru
    LoadedModel(const char* path);

    void draw() override;
};