#include "Mole.h"
#include <cstdio>

Mole::Mole(AbstractModel* model, ShaderProgram* shader, Type moleType)
    : type(moleType), lifetime(3.0f), maxLifetime(3.0f), active(false),
      stencilID(-1), pedestalIndex(-1), targetPedestalIndex(-1),
      popProgress(0.0f), popSpeed(6.0f), popping(true),
      pathMovement(nullptr), moving(false), reachedTarget(false)
{
    // Vytvor vizualny objekt
    visual = new DrawableObject(model, shader);

    // Nastav body a zivotnost podla typu
    points = getPointsForType(moleType);
    maxLifetime = getLifetimeForType(moleType);
    lifetime = maxLifetime;

    // Nastav velkost podla typu
    float scale = getScaleForType(moleType);
    visual->scale(glm::vec3(scale));

    // Vytvor path movement
    pathMovement = new LeafPathMovement();

    // Zaciatok skryty
    visual->visible = false;
}

Mole::~Mole()
{
    // visual sa maze cez ObjectManager
    if (pathMovement) delete pathMovement;
}

void Mole::update(float deltaTime)
{
    if (!active)
    {
        return;
    }

    // FAZA 1: Pop-up animacia na starte
    if (popping)
    {
        popProgress += popSpeed * deltaTime;
        if (popProgress >= 1.0f)
        {
            popProgress = 1.0f;
            popping = false;

            // Zacni pohyb len ak mame validnu cestu (min 2 waypoints)
            if (pathMovement && pathMovement->hasValidPath())
            {
                moving = true;
            }
            else
            {
                // Staticka pozicia - preskoc rovno na reachedTarget
                reachedTarget = true;
                lifetime = maxLifetime;  // Pouzi plny lifetime pre staticke
            }
        }
    }
    // FAZA 2: Pohyb po drahe (parametricky po lomenej ciare)
    else if (moving && pathMovement)
    {
        pathMovement->update(deltaTime);

        // Ked dojde na koniec trajektorie
        if (pathMovement->isFinished())
        {
            moving = false;
            reachedTarget = true;
            lifetime = 0.8f;  // Zostan na cieli kratku chvilu
        }
    }
    // FAZA 3: Cakanie na cieli + pop-down
    else if (reachedTarget)
    {
        lifetime -= deltaTime;

        if (lifetime <= 0.0f)
        {
            // Zacni pop-down animaciu
            popProgress -= popSpeed * deltaTime;
            if (popProgress <= 0.0f)
            {
                popProgress = 0.0f;
                active = false;
                visual->visible = false;
                return;
            }
        }
    }

    // === AKTUALIZUJ POZICIU ===
    visual->getTransformationComposite()->removeTransformations();

    float scale = getScaleForType(type);
    glm::vec3 offset = getOffsetForType(type);
    glm::vec3 pos;

    if (moving && pathMovement)
    {
        // Pocas pohybu - pozicia z parametrickej drahy
        pos = pathMovement->getCurrentPosition();
    }
    else if (reachedTarget)
    {
        // Po dosiahnuti ciela - zostan na CIELOVEJ pozicii
        pos = endPosition;
    }
    else
    {
        // Pop-up faza - startova pozicia
        pos = basePosition;
    }

    // Y offset pre pop animaciu (vyskakovanie/skryvanie)
    float yOffset = popProgress * 0.6f;
    pos += glm::vec3(0.0f, yOffset, 0.0f) + offset;

    visual->translate(pos);
    visual->scale(glm::vec3(scale));

    visual->calculateModelMatrix();
    visual->updateModelMatrix();
}

int Mole::hit()
{
    if (!active || popProgress < 0.5f)
    {
        // Nemoze byt zasiahnuty ak nie je dost viditelny
        return 0;
    }

    active = false;
    visual->visible = false;

    printf("[WHACK!] %s hit! Points: %d\n", getNameForType(type), points);

    return points;
}

void Mole::spawn(glm::vec3 position, int pedestal)
{
    active = true;
    visual->visible = true;
    lifetime = maxLifetime;
    popProgress = 0.0f;
    popping = true;
    moving = false;
    reachedTarget = false;
    basePosition = position;
    endPosition = position;  // Staticka pozicia - start = ciel
    pedestalIndex = pedestal;
    targetPedestalIndex = pedestal;

    // DOLEZITE: Vycisti stare waypoints (pre object pooling)
    if (pathMovement)
    {
        pathMovement->clearWaypoints();
    }

    // Reset transformacie
    visual->getTransformationComposite()->removeTransformations();

    float scale = getScaleForType(type);
    glm::vec3 offset = getOffsetForType(type);

    visual->translate(position + offset);
    visual->scale(glm::vec3(scale));
    visual->calculateModelMatrix();
    visual->updateModelMatrix();

    printf("[SPAWN] %s appeared on pedestal %d!\n", getNameForType(type), pedestal);
}

void Mole::hide()
{
    active = false;
    visual->visible = false;
    popProgress = 0.0f;
}

int Mole::getPointsForType(Type t)
{
    switch (t)
    {
        case Type::SHREK:   return 10;
        case Type::FIONA:   return 25;
        case Type::TOILET:  return 50;
        case Type::CAT:     return -20;
        default:            return 10;
    }
}

float Mole::getLifetimeForType(Type t)
{
    switch (t)
    {
        case Type::SHREK:   return 3.5f;   // Pomaly, dlho viditelny
        case Type::FIONA:   return 2.5f;   // Stredna rychlost
        case Type::TOILET:  return 1.5f;   // Rychly, kratko viditelny
        case Type::CAT:     return 3.0f;   // Stredna (ale nebit!)
        default:            return 3.0f;
    }
}

float Mole::getScaleForType(Type t)
{
    switch (t)
    {
        case Type::SHREK:   return 0.52f;    // +16% +12% zvacsenie
        case Type::FIONA:   return 0.455f;   // +16% +12% zvacsenie
        case Type::TOILET:  return 0.26f;    // +16% +12% zvacsenie
        case Type::CAT:     return 0.0065f;  // +16% +12% zvacsenie
        default:            return 0.26f;
    }
}

glm::vec3 Mole::getOffsetForType(Type t)
{
    // Kompenzacia origin pointu modelov - mensie offsety pre mensie modely
    switch (t)
    {
        case Type::SHREK:   return glm::vec3(-0.4f, 0.1f, 0.4f);   // Mensie offsety
        case Type::FIONA:   return glm::vec3(-0.3f, 0.1f, 0.3f);   // Mensie offsety
        case Type::TOILET:  return glm::vec3(-0.15f, 0.05f, 0.2f); // Mensie offsety
        case Type::CAT:     return glm::vec3(0.0f, 0.1f, 0.0f);
        default:            return glm::vec3(0.0f, 0.1f, 0.0f);
    }
}

const char* Mole::getNameForType(Type t)
{
    switch (t)
    {
        case Type::SHREK:   return "SHREK";
        case Type::FIONA:   return "FIONA";
        case Type::TOILET:  return "TOILET";
        case Type::CAT:     return "CAT";
        default:            return "UNKNOWN";
    }
}

// ========================================
// NOVE: Pohyb po parametrickej trajektorii
// ========================================

void Mole::spawnWithPath(glm::vec3 startPos, glm::vec3 endPos, int startPedestal, int endPedestal)
{
    active = true;
    visual->visible = true;
    lifetime = maxLifetime;
    popProgress = 0.0f;
    popping = true;
    moving = false;
    reachedTarget = false;  // Reset!

    basePosition = startPos;
    endPosition = endPos;   // ULOZ CIELOVU POZICIU!
    pedestalIndex = startPedestal;
    targetPedestalIndex = endPedestal;

    // Vytvor trajektoriu - lomena ciara s miernym oblucikom
    pathMovement->clearWaypoints();

    // Start pozicia
    pathMovement->addWaypoint(startPos);

    // Stredny bod - mierne vyzdvihnuti pre oblucik (skok)
    glm::vec3 midPoint = (startPos + endPos) * 0.5f;
    midPoint.y += 0.8f;  // Vyssi skok pre lepsiu viditelnost
    pathMovement->addWaypoint(midPoint);

    // Cielova pozicia
    pathMovement->addWaypoint(endPos);

    // VYSOKA RYCHLOST - presun trva cca 0.5-1 sekundu
    float moveSpeed = 8.0f;  // Zakladna rychlost
    switch (type)
    {
        case Type::SHREK:   moveSpeed = 6.0f;   break;  // Pomaly ale stale rychly
        case Type::FIONA:   moveSpeed = 8.0f;   break;  // Stredna
        case Type::TOILET:  moveSpeed = 12.0f;  break;  // Velmi rychly (50 bodov!)
        case Type::CAT:     moveSpeed = 7.0f;   break;
        default:            moveSpeed = 8.0f;
    }
    pathMovement->setSpeed(moveSpeed);
    pathMovement->setLoop(false);
    pathMovement->setPingPong(false);
    pathMovement->setT(0.0f);

    // Reset transformacie
    visual->getTransformationComposite()->removeTransformations();

    float scale = getScaleForType(type);
    glm::vec3 offset = getOffsetForType(type);

    visual->translate(startPos + offset);
    visual->scale(glm::vec3(scale));
    visual->calculateModelMatrix();
    visual->updateModelMatrix();

    printf("[SPAWN] %s: pedestal %d -> %d (speed: %.1f)\n",
           getNameForType(type), startPedestal, endPedestal, moveSpeed);
}

void Mole::setPath(std::vector<glm::vec3> waypoints)
{
    if (pathMovement)
    {
        pathMovement->setWaypoints(waypoints);
    }
}

void Mole::setMoveSpeed(float speed)
{
    if (pathMovement)
    {
        pathMovement->setSpeed(speed);
    }
}

bool Mole::hasReachedTarget() const
{
    if (pathMovement)
    {
        return pathMovement->isFinished();
    }
    return false;
}