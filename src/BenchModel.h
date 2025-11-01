#pragma once

#include "AbstractModel.h"
#include "models/bench.h"  // Obsahuje pole benchVertices[] s vertex dátami

class BenchModel : public AbstractModel
{
public:
    BenchModel();
    void draw() override;
};