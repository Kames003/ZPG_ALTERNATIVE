#pragma once

#include "AbstractModel.h"
#include "models/tree.h"  // Obsahuje pole tree[] s vertex dátami

// Konkrétny model pre strom
// Identická štruktúra ako SphereModel, len iné dáta a počet vrcholov
class TreeModel : public AbstractModel
{
public:
    TreeModel();
    void draw() override;
};