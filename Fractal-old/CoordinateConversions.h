#pragma once
#include <glm\vec3.hpp>

// Defines common coordinate conversions
class CoordinateConversions
{
public:
    static glm::vec3 EuclidianToSpherical(const glm::vec3 point);
    static glm::vec3 SphericalToEuclidian(const glm::vec3 point);

    static glm::vec3 EuclidianToCylindrical(const glm::vec3 point);
    static glm::vec3 CylindricalToEuclidian(const glm::vec3 point);

    static glm::vec3 SphericalToCylindrical(const glm::vec3 point);
    static glm::vec3 CylindricalToSpherical(const glm::vec3 point);
};

