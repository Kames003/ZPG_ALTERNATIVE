#include "Hammer.h"
#include <cstdio>

Hammer::Hammer(AbstractModel* model, ShaderProgram* shader)
    : position(0.0f), visible(false), visibleTimer(0.0f),
      visibleDuration(0.3f), scale(0.05f)
{
    visual = new DrawableObject(model, shader);
    visual->visible = false;  // Zaciatok - skryte

    printf("[Hammer] Created (hidden by default)\n");
}

Hammer::~Hammer()
{
    // visual sa maze cez ObjectManager
}

void Hammer::update(float deltaTime)
{
    if (visible)
    {
        visibleTimer -= deltaTime;

        if (visibleTimer <= 0.0f)
        {
            // Cas vyprsal - skry kladivo
            visible = false;
            visual->visible = false;
        }
    }
}

void Hammer::strike(glm::vec3 pos)
{
    position = pos;
    visible = true;
    visibleTimer = visibleDuration;
    visual->visible = true;

    // Aktualizuj transformaciu - kladivo horizontalne
    visual->getTransformationComposite()->removeTransformations();

    // Pozicia uderu
    visual->translate(position + glm::vec3(0.0f, 0.8f, 0.0f));

    // Scale
    visual->scale(glm::vec3(scale));

    visual->calculateModelMatrix();
    visual->updateModelMatrix();

    printf("[Hammer] STRIKE at (%.2f, %.2f, %.2f)!\n", pos.x, pos.y, pos.z);
}