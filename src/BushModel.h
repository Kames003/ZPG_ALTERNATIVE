#pragma once

#include "AbstractModel.h"
#include "models/bushes.h"

class BushModel : public AbstractModel
{
public:
    BushModel();
    void draw() override;
};