#pragma once

#include "AbstractModel.h"
#include "models/plain_texture.h"


class PlainTextureModel : public AbstractModel
{
public:
    PlainTextureModel();
    void draw() override;
};
