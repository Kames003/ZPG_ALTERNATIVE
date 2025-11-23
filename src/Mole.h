#pragma once

#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include "LeafPathMovement.h"
#include <glm/glm.hpp>

/**
 * @brief WHAC-A-MOLE - Postava ktora sa pohybuje medzi podstavcami
 *
 * Mole reprezentuje postavu (Shrek, Fiona, WC, Cat) ktora sa objavi
 * na podstavci a pohybuje sa po trajektorii k cielu.
 * Hrac ju musi "udriet" kliknutim.
 *
 * Typy postav:
 * - SHREK     - 10 bodov, pomaly
 * - FIONA     - 25 bodov, stredna rychlost
 * - TOILET    - 50 bodov, rychly (vzacny!)
 * - CAT       - -20 bodov (nebit macku!)
 *
 * POHYB: Parametricky pohyb po lomenej ciare (LeafPathMovement)
 */
class Mole
{
public:
    enum class Type
    {
        SHREK,      // Zeleny ogre, 10 bodov
        FIONA,      // Princezna, 25 bodov
        TOILET,     // WC, 50 bodov (vzacne!)
        CAT         // Macka, -20 bodov (nebit!)
    };

private:
    DrawableObject* visual;
    Type type;
    int points;
    float lifetime;         // Kolko sekund je viditelny
    float maxLifetime;
    bool active;
    int stencilID;
    int pedestalIndex;      // Startovaci podstavec (0-5)
    int targetPedestalIndex; // Cielovy podstavec

    // Animacia vyskakovania
    float popProgress;      // 0.0 = skryty, 1.0 = plne viditelny
    float popSpeed;         // Rychlost animacie
    bool popping;           // Ci prave vyskakuje alebo sa skryva
    glm::vec3 basePosition; // Pozicia startovacie podstavca
    glm::vec3 endPosition;  // Pozicia cieloveho podstavca

    // NOVE: Pohyb po trajektorii
    LeafPathMovement* pathMovement;  // Parametricky pohyb po lomenej ciare
    bool moving;                      // Ci sa pohybuje po drahe
    bool reachedTarget;               // Ci dosiahol ciel

public:
    Mole(AbstractModel* model, ShaderProgram* shader, Type moleType);
    ~Mole();

    // Nastavenie
    void setLifetime(float seconds) { maxLifetime = seconds; lifetime = seconds; }
    void setPedestalIndex(int index) { pedestalIndex = index; }
    void setBasePosition(glm::vec3 pos) { basePosition = pos; }

    // Update - volat kazdy frame
    void update(float deltaTime);

    // Akcie
    int hit();  // Vrati body a deaktivuje
    void spawn(glm::vec3 position, int pedestal);
    void hide();

    // NOVE: Pohyb po trajektorii
    void spawnWithPath(glm::vec3 startPos, glm::vec3 endPos, int startPedestal, int endPedestal);
    void setPath(std::vector<glm::vec3> waypoints);
    void setMoveSpeed(float speed);
    bool isMoving() const { return moving; }
    bool hasReachedTarget() const;
    int getTargetPedestalIndex() const { return targetPedestalIndex; }

    // Gettery
    bool isActive() const { return active; }
    bool isExpired() const { return lifetime <= 0.0f; }
    bool isFullyVisible() const { return popProgress >= 1.0f; }
    int getPoints() const { return points; }
    Type getType() const { return type; }
    DrawableObject* getVisual() { return visual; }
    int getStencilID() const { return stencilID; }
    void setStencilID(int id) { stencilID = id; }
    int getPedestalIndex() const { return pedestalIndex; }
    float getRemainingLifetime() const { return lifetime; }

    // Staticke pomocne metody
    static int getPointsForType(Type t);
    static float getLifetimeForType(Type t);
    static float getScaleForType(Type t);
    static glm::vec3 getOffsetForType(Type t);  // Offset pre centrovanie
    static const char* getNameForType(Type t);
};