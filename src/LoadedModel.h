#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "AbstractModel.h"

/**
 * @brief Model načítaný zo súboru (OBJ, FBX, atď.) pomocou knižnice ASSIMP
 *
 * Načíta geometriu (pozície, normály, UV) zo súboru.
 * Pre materiály použite MaterialManager a MTLLoader samostatne.
 */

class LoadedModel : public AbstractModel
{
protected:
    int count;  // Počet vrcholov na vykreslenie

public:
    // Konštruktor - načíta model zo súboru
    LoadedModel(const char* path);

    void draw() override;
};