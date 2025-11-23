#pragma once

#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

/**
 * @brief Kladivo pre WHAC-A-MOLE hru
 *
 * Kladivo je skryte. Pri kliknuti sa objavi na pozicii
 * kurzora v "uderenom" uhle a po kratkom case zmizne.
 */
class Hammer
{
private:
    DrawableObject* visual;

    // Pozicia
    glm::vec3 position;         // Pozicia uderu v 3D

    // Viditelnost
    bool visible;               // Ci je kladivo viditelne
    float visibleTimer;         // Ako dlho je este viditelne
    float visibleDuration;      // Ako dlho zostane viditelne po udere

    // Velkost
    float scale;

public:
    Hammer(AbstractModel* model, ShaderProgram* shader);
    ~Hammer();

    // Nastavenie
    void setScale(float s) { scale = s; }
    void setVisibleDuration(float d) { visibleDuration = d; }

    // Update - volat kazdy frame
    void update(float deltaTime);

    // Uder na danej pozicii - zobrazi kladivo
    void strike(glm::vec3 pos);

    // Gettery
    bool isVisible() const { return visible; }
    DrawableObject* getVisual() { return visual; }
};
