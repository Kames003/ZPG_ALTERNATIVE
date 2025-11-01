#pragma once

#include "AbstractModel.h"
#include "models/bushes.h"  // Obsahuje pole bushes[] s vertex dátami

class BushModel : public AbstractModel
{
public:
    BushModel();
    void draw() override;
};