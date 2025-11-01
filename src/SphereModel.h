#pragma once

#include "AbstractModel.h"
#include "models/sphere.h"  // Obsahuje pole sphere[] s vertex dátami

// Konkrétny model pre guľu/sféru
// Načítava dáta zo statického poľa definovaného v models/sphere.h
class SphereModel : public AbstractModel
{
public:
    SphereModel();
    void draw() override;  // Implementácia draw() z AbstractModel
};