#pragma once

#include "TransformationComponent.h"
#include <vector>


class LeafPathMovement : public TransformationComponent
{
private:
    std::vector<glm::vec3> waypoints;
    float t;
    float speed;
    bool loop;
    bool pingPong;
    bool forward;
    float totalLength;

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
