#include "LeafPathMovement.h"

LeafPathMovement::LeafPathMovement()
    : t(0.0f), speed(1.0f), loop(false), pingPong(false), forward(true), totalLength(0.0f)
{
    matrix = glm::mat4(1.0f);
}

LeafPathMovement::LeafPathMovement(std::vector<glm::vec3> points)
    : t(0.0f), speed(1.0f), loop(false), pingPong(false), forward(true), totalLength(0.0f)
{
    matrix = glm::mat4(1.0f);
    setWaypoints(points);
}

void LeafPathMovement::addWaypoint(glm::vec3 point)
{
    waypoints.push_back(point);
    calculateTotalLength();
}

void LeafPathMovement::clearWaypoints()
{
    waypoints.clear();
    totalLength = 0.0f;
    t = 0.0f;
}

void LeafPathMovement::setWaypoints(std::vector<glm::vec3> points)
{
    waypoints = points;
    calculateTotalLength();
}

void LeafPathMovement::calculateTotalLength()
{
    totalLength = 0.0f;
    for (size_t i = 1; i < waypoints.size(); i++)
    {
        totalLength += glm::length(waypoints[i] - waypoints[i - 1]);
    }
}

glm::vec3 LeafPathMovement::calculatePosition()
{
    if (waypoints.empty())
    {
        return glm::vec3(0.0f);
    }

    if (waypoints.size() == 1)
    {
        return waypoints[0];
    }

    // Vypocitaj poziciu na drahe podla parametra t
    float totalT = t * (waypoints.size() - 1);
    int segmentIndex = static_cast<int>(totalT);
    float segmentT = totalT - segmentIndex;

    // Clamp na posledny segment
    if (segmentIndex >= static_cast<int>(waypoints.size()) - 1)
    {
        segmentIndex = waypoints.size() - 2;
        segmentT = 1.0f;
    }

    // Linearna interpolacia medzi waypointmi
    glm::vec3 start = waypoints[segmentIndex];
    glm::vec3 end = waypoints[segmentIndex + 1];

    return glm::mix(start, end, segmentT);
}

glm::vec3 LeafPathMovement::getCurrentPosition() const
{
    if (waypoints.empty())
    {
        return glm::vec3(0.0f);
    }

    if (waypoints.size() == 1)
    {
        return waypoints[0];
    }

    float totalT = t * (waypoints.size() - 1);
    int segmentIndex = static_cast<int>(totalT);
    float segmentT = totalT - segmentIndex;

    if (segmentIndex >= static_cast<int>(waypoints.size()) - 1)
    {
        segmentIndex = waypoints.size() - 2;
        segmentT = 1.0f;
    }

    glm::vec3 start = waypoints[segmentIndex];
    glm::vec3 end = waypoints[segmentIndex + 1];

    return glm::mix(start, end, segmentT);
}

void LeafPathMovement::update(float deltaTime)
{
    if (waypoints.size() < 2)
    {
        return;
    }

    // Vypocitaj zmenu t podla rychlosti a dlzky drahy
    float deltaT = (speed * deltaTime) / totalLength;

    if (forward)
    {
        t += deltaT;
    }
    else
    {
        t -= deltaT;
    }

    // Spracuj koniec drahy
    if (t >= 1.0f)
    {
        if (pingPong)
        {
            t = 1.0f;
            forward = false;
        }
        else if (loop)
        {
            t = 0.0f;
        }
        else
        {
            t = 1.0f;
        }
    }
    else if (t <= 0.0f)
    {
        if (pingPong)
        {
            t = 0.0f;
            forward = true;
        }
        else if (loop)
        {
            t = 1.0f;
        }
        else
        {
            t = 0.0f;
        }
    }

    // Aktualizuj transformacnu maticu
    glm::vec3 position = calculatePosition();
    matrix = glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 LeafPathMovement::resultMatrix()
{
    return matrix;
}
