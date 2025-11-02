#pragma once

#include "AbstractModel.h"
#include "models/triangle.h"  // ✅ Tvoj existujúci súbor!


class TriangleModel : public AbstractModel
{
public:
    TriangleModel();
    void draw() override;
};