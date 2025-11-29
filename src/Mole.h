#pragma once

#include "DrawableObject.h"
#include "AbstractModel.h"
#include "ShaderProgram.h"
#include "LeafPathMovement.h"
#include <glm/glm.hpp>


class Mole
{
public:
    enum class Type
    {
        SHREK,
        FIONA,
        TOILET,
        CAT
    };

private:
    DrawableObject* visual;
    Type type;
    int points;
    float lifetime;
    float maxLifetime;
    bool active;
    int stencilID;
    int pedestalIndex;
    int targetPedestalIndex;

    // Animacia vyskakovania
    float popProgress;
    float popSpeed;
    bool popping;
    glm::vec3 basePosition;
    glm::vec3 endPosition;


    LeafPathMovement* pathMovement;
    bool moving;
    bool reachedTarget;

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