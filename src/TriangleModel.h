#pragma once

#include "AbstractModel.h"
#include "models/triangle.h"  // ✅ Tvoj existujúci súbor!

/**
 * @brief Model jednoduchého trojuholníka
 *
 * Načítava dáta zo statického poľa triangle[] definovaného v models/triangle.h
 * Formát: [x, y, z, nx, ny, nz] - 3 vrcholy = 18 floatov
 */
class TriangleModel : public AbstractModel
{
public:
    TriangleModel();
    void draw() override;
};