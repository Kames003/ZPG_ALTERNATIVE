#pragma once

#include "AbstractModel.h"
#include "models/plain_texture.h"

/**
 * @brief Model roviny (plane) s podporou textúr
 * 
 * Rovina s UV textúrovými súradnicami pre aplikáciu textúr.
 * UV súradnice sú nastavené na 0-10 pre hustejšie opakovanie textúry.
 */
class PlainTextureModel : public AbstractModel
{
public:
    PlainTextureModel();
    void draw() override;
};
