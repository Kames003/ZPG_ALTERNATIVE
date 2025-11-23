#pragma once

#include "DrawableObject.h"
#include "LeafPathMovement.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"

/**
 * @brief DUCK HUNTER - Kacka/terc pre arkadovu hru
 *
 * Target reprezentuje lietajucu kacku ktoru moze hrac zasianut.
 * Kazda kacka ma:
 * - Vizualny objekt (DrawableObject)
 * - Pohyb po drahe (LeafPathMovement)
 * - Bodovu hodnotu (points)
 * - Zivotnost (lifetime)
 *
 * Typy kaciek:
 * - BROWN (hneda)     - 10 bodov, pomala
 * - GREEN (zelena)    - 25 bodov, stredna
 * - GOLDEN (zlata)    - 50 bodov, rychla
 * - RED_BONUS (cervena) - 100 bodov, velmi rychla
 * - BLUE_PROTECTED (modra) - -30 bodov (chraneny vtak!)
 */
class Target
{
public:
    enum class Type
    {
        BROWN,          // Hneda kacka, 10 bodov
        GREEN,          // Zelena kacka, 25 bodov
        GOLDEN,         // Zlata kacka, 50 bodov
        RED_BONUS,      // Cervena bonus kacka, 100 bodov
        BLUE_PROTECTED  // Modry chraneny vtak, -30 bodov
    };

private:
    DrawableObject* visual;
    LeafPathMovement* movement;
    Type type;
    int points;
    float lifetime;       // Kolko sekund existuje
    float maxLifetime;    // Maximalny cas
    bool active;
    int stencilID;        // ID pre picking

public:
    Target(AbstractModel* model, ShaderProgram* shader, Type targetType);
    ~Target();

    // Nastavenie drahy
    void setPath(std::vector<glm::vec3> waypoints);
    void setSpeed(float speed);
    void setLoop(bool loop);
    void setPingPong(bool pingPong);

    // Nastavenie zivotnosti
    void setLifetime(float seconds) { maxLifetime = seconds; lifetime = seconds; }

    // Update - volat kazdy frame
    void update(float deltaTime);

    // Akcie
    int hit();  // Vrati body a deaktivuje terc
    void spawn(glm::vec3 startPosition);

    // Gettery
    bool isActive() const { return active; }
    bool isExpired() const { return lifetime <= 0.0f; }
    int getPoints() const { return points; }
    Type getType() const { return type; }
    DrawableObject* getVisual() { return visual; }
    int getStencilID() const { return stencilID; }
    void setStencilID(int id) { stencilID = id; }
    glm::vec3 getPosition() const { return movement->getCurrentPosition(); }
    float getRemainingLifetime() const { return lifetime; }
    float getLifetimePercent() const { return lifetime / maxLifetime; }

    // Staticke pomocne metody
    static glm::vec3 getColorForType(Type t);
    static int getPointsForType(Type t);
};
