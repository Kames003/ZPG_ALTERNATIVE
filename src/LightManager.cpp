#include "LightManager.h"

void LightManager::addPointLight(PointLight* l)
{
    pointlights.push_back(l);
}

void LightManager::addSpotlight(Spotlight* l)
{
    spotlights.push_back(l);
}

void LightManager::addSpotlightCamera(SpotlightCamera* l)
{
    this->spotlightCamera = l;
}

void LightManager::addDirectionalLight(DirectionalLight* l)
{
    this->directional = l;
}

int LightManager::getPointLightsSize()
{
    return pointlights.size();
}

int LightManager::getSpotlightsSize()
{
    return spotlights.size();
}

PointLight* LightManager::getPointLight(int index)
{
    return pointlights[index];
}

Spotlight* LightManager::getSpotlight(int index)
{
    return spotlights[index];
}

SpotlightCamera* LightManager::getSpotlightCamera()
{
    return this->spotlightCamera;
}

DirectionalLight* LightManager::getDirectionalLight()
{
    return this->directional;
}