#include "SphereShape.h"

SphereShape::SphereShape(float angularResolution)
    : EllipseShape(angularResolution, glm::vec3(1.0f, 1.0f, 1.0f))
{
}