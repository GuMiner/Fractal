#include <cmath>
#include <glm\gtx\norm.hpp>
#include "CoordinateConversions.h"

glm::vec3 CoordinateConversions::EuclidianToSpherical(const glm::vec3 point)
{
    float distance = glm::length(point);
    return glm::vec3(distance, std::acos(point.z / distance), std::atan2(point.y, point.x));
}

glm::vec3 CoordinateConversions::SphericalToEuclidian(const glm::vec3 point)
{
    return glm::vec3(
        point.x * std::sin(point.y) * std::cos(point.z),
        point.x * std::sin(point.y) * std::sin(point.z),
        point.x * std::cos(point.y));

}

glm::vec3 CoordinateConversions::EuclidianToCylindrical(const glm::vec3 point)
{
    return glm::vec3(point.x * std::cos(point.y), point.x * std::sin(point.y), point.z);
}

glm::vec3 CoordinateConversions::CylindricalToEuclidian(const glm::vec3 point)
{
    float distance = glm::length(glm::vec2(point.x, point.y));
    return glm::vec3(distance, std::atan2(point.y, point.x), point.z);
}

glm::vec3 CoordinateConversions::SphericalToCylindrical(const glm::vec3 point)
{
    return EuclidianToCylindrical(SphericalToEuclidian(point));
}

glm::vec3 CoordinateConversions::CylindricalToSpherical(const glm::vec3 point)
{
    return EuclidianToSpherical(CylindricalToEuclidian(point));
}