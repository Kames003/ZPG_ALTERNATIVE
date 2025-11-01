#include "Firefly.h"
#include <cmath>
#include <cstdlib>

Firefly::Firefly(glm::vec3 position, 
                 AbstractModel* model, 
                 ShaderProgram* shader,
                 glm::vec3 lightColor)
{
    this->centerPosition = position;
    this->color = lightColor;
    
    // ========================================
    // NÁHODNÉ PARAMETRY PRO KAŽDOU SVĚTLUŠKU
    // ========================================
    // Orbit radius: 0.5 - 1.5 metrů
    this->orbitRadius = 0.5f + static_cast<float>(rand() % 100) / 100.0f;
    
    // Orbit speed: 0.3 - 0.8 (radiánů za sekundu)
    this->orbitSpeed = 0.3f + static_cast<float>(rand() % 100) / 200.0f;
    
    // Vertical speed: 0.2 - 0.5 (radiánů za sekundu)
    this->verticalSpeed = 0.2f + static_cast<float>(rand() % 100) / 300.0f;
    
    // Phase: 0 - 2π (náhodný fázový posun)
    this->phase = static_cast<float>(rand() % 360) * (3.14159f / 180.0f);
    
    // Blink speed: 2.0 - 4.0 Hz
    this->blinkSpeed = 2.0f + static_cast<float>(rand() % 100) / 50.0f;
    
    // ========================================
    // INTENZITA BLIKÁNÍ
    // ========================================
    this->minIntensity = 0.3f;
    this->maxIntensity = 1.0f;
    this->intensity = maxIntensity;
    
    // ========================================
    // SVĚTLO - NASTAVENÍ PRO SVĚTLUŠKY
    // ========================================
    // Útlum nastaven tak, aby světlo dosvítilo pouze ~2-3 metry
    // - constant: 1.0 (základní útlum)
    // - linear: 0.7 (silný útlum se vzdáleností)
    // - quadratic: 1.8 (velmi silný útlum - realistické pro malý zdroj)
    this->light = new PointLight(
        position,
        1.0f,    // constant
        0.7f,    // linear
        1.8f,    // quadratic
        lightColor
    );
    
    // ========================================
    // VIZUALIZACE - MALÁ BÍLÁ KULIČKA
    // ========================================
    // Vytvoříme DrawableObject s bílou barvou
    this->visual = new DrawableObject(model, shader, glm::vec3(1.0f, 1.0f, 1.0f));
    
    // Nastavíme počáteční transformace
    visual->translate(position);
    visual->scale(glm::vec3(0.08f, 0.08f, 0.08f));  // Velmi malá kulička
    visual->calculateModelMatrix();
    visual->updateModelMatrix();
}

void Firefly::updateAnimation(float time)
{
    // Aplikace fázového posunu
    float t = time + phase;
    
    // ========================================
    // VÝPOČET NOVÉ POZICE
    // ========================================
    // Kruhový pohyb v XZ rovině
    float angle = t * orbitSpeed;
    float x = centerPosition.x + orbitRadius * cos(angle);
    float z = centerPosition.z + orbitRadius * sin(angle);
    
    // Sinusový pohyb ve výšce (Y)
    float y = centerPosition.y + 0.5f * sin(t * verticalSpeed);
    
    glm::vec3 newPosition(x, y, z);
    
    // ========================================
    // BLIKÁNÍ - SINUSOVÝ PŘECHOD INTENZITY
    // ========================================
    // Intenzita osciluje mezi minIntensity a maxIntensity
    intensity = minIntensity + 
               (maxIntensity - minIntensity) * 
               (0.5f + 0.5f * sin(t * blinkSpeed));
    
    // ========================================
    // AKTUALIZACE SVĚTLA
    // ========================================
    // Barva světla = základní barva × aktuální intenzita
    light->color = color * intensity;
    
    // Pozice světla
    light->position = newPosition;
    
    // ========================================
    // AKTUALIZACE VIZUALIZACE
    // ========================================
    // DŮLEŽITÉ: Musíme znovu přidat transformace každý frame!
    // (TransformationComposite se vyprázdní po calculateModelMatrix)
    visual->translate(newPosition);
    visual->scale(glm::vec3(0.08f, 0.08f, 0.08f));
    visual->calculateModelMatrix();
    visual->updateModelMatrix();
}

void Firefly::setCenterPosition(glm::vec3 pos)
{
    this->centerPosition = pos;
}

void Firefly::setOrbitRadius(float radius)
{
    this->orbitRadius = radius;
}

void Firefly::setSpeed(float speed)
{
    this->orbitSpeed = speed;
}

PointLight* Firefly::getLight()
{
    return this->light;
}

DrawableObject* Firefly::getVisual()
{
    return this->visual;
}

void Firefly::setVisualVisible(bool visible)
{
    this->visual->visible = visible;
}