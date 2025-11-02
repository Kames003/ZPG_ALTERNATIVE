#pragma once

#include "AbstractModel.h"
#include "models/tree.h"

class TreeModel : public AbstractModel
{
public:
    TreeModel();
    void draw() override;
};