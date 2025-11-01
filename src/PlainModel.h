#pragma once

#include "AbstractModel.h"
#include "models/plain.h"  // Obsahuje pole plain[] s vertex dátami

class PlainModel : public AbstractModel
{
public:
    PlainModel();
    void draw() override;
};