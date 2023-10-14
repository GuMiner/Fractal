#include <glm\vec3.hpp>
#include "DistanceActivator.h"
#include "WireCubeGenerator.h"
#include "WireCube.h"

WireCube::WireCube(GeometryGenerationScheduler* scheduler) 
    : BaseObjectType(new DistanceActivator(30.0f), new WireCubeGenerator(), scheduler)
{
}

std::string WireCube::GetName()
{
    return "Wire Cube";
}