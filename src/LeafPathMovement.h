#pragma once

#include "TransformationComponent.h"
#include <vector>

/**
 * @brief Pohyb po usecke alebo lomenej ciare (polyline)
 *
 * Transformacia pre parametricky pohyb objektu po definovanej drahe.
 * Draha je zadana ako zoznam waypointov (bodov).
 * Parameter t urcuje poziciu na drahe (0.0 = zaciatok, 1.0 = koniec).
 *
 * Pouzitie:
 * @code
 * LeafPathMovement* path = new LeafPathMovement();
 * path->addWaypoint(glm::vec3(0, 0, 0));   // Start
 * path->addWaypoint(glm::vec3(10, 5, 0));  // Bod 1
 * path->addWaypoint(glm::vec3(20, 0, 0));  // Koniec
 * path->setSpeed(0.5f);
 * path->setLoop(true);
 *
 * // V renderFrame():
 * path->update(deltaTime);
 * object->getTransformationComposite()->addTransformation(path);
 * @endcode
 */
class LeafPathMovement : public TransformationComponent
{
private:
    std::vector<glm::vec3> waypoints;  // Body drahy
    float t;                            // Parameter 0.0 - 1.0
    float speed;                        // Rychlost pohybu (jednotky za sekundu)
    bool loop;                          // Opakovat drahu?
    bool pingPong;                      // Tam a spat?
    bool forward;                       // Smer pohybu (pre pingPong)
    float totalLength;                  // Celkova dlzka drahy

    void calculateTotalLength();
    glm::vec3 calculatePosition();

public:
    LeafPathMovement();
    LeafPathMovement(std::vector<glm::vec3> points);

    // Pridanie waypointov
    void addWaypoint(glm::vec3 point);
    void clearWaypoints();
    void setWaypoints(std::vector<glm::vec3> points);

    // Nastavenia pohybu
    void setSpeed(float s) { speed = s; }
    void setLoop(bool l) { loop = l; }
    void setPingPong(bool pp) { pingPong = pp; }
    void setT(float newT) { t = glm::clamp(newT, 0.0f, 1.0f); }

    // Gettery
    float getT() const { return t; }
    float getSpeed() const { return speed; }
    bool isFinished() const { return !loop && !pingPong && t >= 1.0f; }
    glm::vec3 getCurrentPosition() const;
    size_t getWaypointCount() const { return waypoints.size(); }
    bool hasValidPath() const { return waypoints.size() >= 2; }

    // Update - volat kazdy frame
    void update(float deltaTime);

    // TransformationComponent interface
    glm::mat4 resultMatrix() override;
};
