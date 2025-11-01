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
    // ✅ NOVÉ: VARIACE TYPU SVĚTLUŠKY
    // ========================================
    // Každá světluška má náhodný typ (0-2)
    int fireflyType = rand() % 3;

    float constant, linear, quadratic;
    glm::vec3 visualScale;

    switch(fireflyType)
    {
        case 0: // SILNÁ světluška - dosvit ~3-4 metry
            constant = 1.0f;
            linear = 0.5f;      // Slabší útlum
            quadratic = 1.2f;   // Slabší útlum
            visualScale = glm::vec3(0.10f, 0.10f, 0.10f);  // Větší kulička
            // Vizualizace: trochu světlejší/žlutavější
            visual = new DrawableObject(model, shader, glm::vec3(1.0f, 1.0f, 0.9f));
            break;

        case 1: // NORMÁLNÍ světluška - dosvit ~2-3 metry
            constant = 1.0f;
            linear = 0.7f;      // Střední útlum
            quadratic = 1.8f;   // Střední útlum
            visualScale = glm::vec3(0.08f, 0.08f, 0.08f);  // Normální
            // Vizualizace: bílá
            visual = new DrawableObject(model, shader, glm::vec3(1.0f, 1.0f, 1.0f));
            break;

        case 2: // SLABÁ světluška - dosvit ~1-2 metry
            constant = 1.0f;
            linear = 0.9f;      // Silný útlum
            quadratic = 2.5f;   // Velmi silný útlum
            visualScale = glm::vec3(0.06f, 0.06f, 0.06f);  // Menší kulička
            // Vizualizace: slabší/namodralejší
            visual = new DrawableObject(model, shader, glm::vec3(0.9f, 0.9f, 0.7f));
            break;
    }

    // ✅ Uložíme si scale pro pozdější použití
    this->visualScale = visualScale;

    // ========================================
    // SVĚTLO - NASTAVENÍ PRO SVĚTLUŠKY
    // ========================================
    this->light = new PointLight(
        position,
        constant,   // ✅ Variabilní podle typu
        linear,     // ✅ Variabilní podle typu
        quadratic,  // ✅ Variabilní podle typu
        lightColor
    );

    // ========================================
    // DOKONČENÍ VIZUALIZACE
    // ========================================
    visual->translate(position);
    visual->scale(visualScale);  // ✅ Použijeme uložený scale
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
    visual->scale(this->visualScale);  // ✅ Použijeme uložený scale

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