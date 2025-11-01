#pragma once

#include "AbstractModel.h"
#include "models/gift.h"  // Obsahuje pole gift[] s vertex dátami

class GiftModel : public AbstractModel
{
public:
    GiftModel();
    void draw() override;
};