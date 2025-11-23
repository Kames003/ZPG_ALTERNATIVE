#include "Target.h"
#include <cstdio>

Target::Target(AbstractModel* model, ShaderProgram* shader, Type targetType)
    : type(targetType), lifetime(10.0f), maxLifetime(10.0f), active(true), stencilID(-1)
{
    // Vytvor vizualny objekt
    visual = new DrawableObject(model, shader);

    // Nastav farbu podla typu
    visual->setColor(getColorForType(targetType));

    // Nastav body podla typu
    points = getPointsForType(targetType);

    // Vytvor pohyb
    movement = new LeafPathMovement();

    // Nastav scale podla typu kacky
    float scale = 0.3f;
    switch (targetType)
    {
        case Type::RED_BONUS:
            scale = 0.5f;   // Velka cervena bonus kacka
            break;
        case Type::GOLDEN:
            scale = 0.35f;  // Zlata kacka
            break;
        case Type::GREEN:
            scale = 0.3f;   // Zelena kacka
            break;
        case Type::BLUE_PROTECTED:
            scale = 0.25f;  // Maly chraneny vtak
            break;
        case Type::BROWN:
        default:
            scale = 0.3f;   // Hneda kacka
    }

    visual->scale(glm::vec3(scale));
}

Target::~Target()
{
    delete movement;
    // visual sa maze cez ObjectManager
}

void Target::setPath(std::vector<glm::vec3> waypoints)
{
    movement->setWaypoints(waypoints);
}

void Target::setSpeed(float speed)
{
    movement->setSpeed(speed);
}

void Target::setLoop(bool loop)
{
    movement->setLoop(loop);
}

void Target::setPingPong(bool pingPong)
{
    movement->setPingPong(pingPong);
}

void Target::update(float deltaTime)
{
    if (!active)
    {
        return;
    }

    // Aktualizuj zivotnost
    lifetime -= deltaTime;

    if (lifetime <= 0.0f)
    {
        active = false;
        visual->visible = false;
        return;
    }

    // Aktualizuj pohyb
    movement->update(deltaTime);

    // Aktualizuj poziciu vizualu
    // Musime preratat transformacie
    visual->getTransformationComposite()->removeTransformations();

    // Pridaj poziciu z pohybu
    glm::vec3 pos = movement->getCurrentPosition();
    visual->translate(pos);

    // Pridaj scale podla typu kacky
    float scale = 0.3f;
    switch (type)
    {
        case Type::RED_BONUS:
            scale = 0.5f;
            break;
        case Type::GOLDEN:
            scale = 0.35f;
            break;
        case Type::GREEN:
            scale = 0.3f;
            break;
        case Type::BLUE_PROTECTED:
            scale = 0.25f;
            break;
        case Type::BROWN:
        default:
            scale = 0.3f;
    }
    visual->scale(glm::vec3(scale));

    // Pridaj rotaciu pre efekt (terc sa otaca)
    float rotationSpeed = 90.0f;  // stupnov za sekundu
    static float totalRotation = 0.0f;
    totalRotation += rotationSpeed * deltaTime;
    visual->rotate(totalRotation, glm::vec3(0.0f, 1.0f, 0.0f));

    visual->calculateModelMatrix();
    visual->updateModelMatrix();
}

int Target::hit()
{
    if (!active)
    {
        return 0;
    }

    active = false;
    visual->visible = false;

    printf("[Target] HIT! Type: %d, Points: %d\n",
           static_cast<int>(type), points);

    return points;
}

void Target::spawn(glm::vec3 startPosition)
{
    active = true;
    visual->visible = true;
    lifetime = maxLifetime;

    // Reset pohybu
    movement->setT(0.0f);

    printf("[Target] Spawned at (%.1f, %.1f, %.1f), Type: %d, Points: %d\n",
           startPosition.x, startPosition.y, startPosition.z,
           static_cast<int>(type), points);
}

glm::vec3 Target::getColorForType(Type t)
{
    switch (t)
    {
        case Type::BROWN:
            return glm::vec3(0.6f, 0.4f, 0.2f);   // Hneda kacka
        case Type::GREEN:
            return glm::vec3(0.2f, 0.7f, 0.3f);   // Zelena kacka
        case Type::GOLDEN:
            return glm::vec3(1.0f, 0.85f, 0.0f);  // Zlata kacka
        case Type::RED_BONUS:
            return glm::vec3(1.0f, 0.2f, 0.2f);   // Cervena bonus kacka
        case Type::BLUE_PROTECTED:
            return glm::vec3(0.3f, 0.5f, 1.0f);   // Modry chraneny vtak
        default:
            return glm::vec3(0.6f, 0.4f, 0.2f);   // Default hneda
    }
}

int Target::getPointsForType(Type t)
{
    switch (t)
    {
        case Type::BROWN:
            return 10;
        case Type::GREEN:
            return 25;
        case Type::GOLDEN:
            return 50;
        case Type::RED_BONUS:
            return 100;
        case Type::BLUE_PROTECTED:
            return -30;
        default:
            return 10;
    }
}
