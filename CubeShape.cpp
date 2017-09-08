#define _USE_MATH_DEFINES
#include <cmath>
#include <glm\gtx\transform.hpp>
#include "PlanePrimitive.h"
#include "CubeShape.h"

CubeShape::CubeShape(int divisions)
    : divisions(divisions)
{
}

CoreGeometry CubeShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    float ninetyDegrees = M_PI_2;
    PlanePrimitive::AddPlane(vertices, normals, glm::mat3());
    PlanePrimitive::AddPlane(vertices, normals, glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));
    PlanePrimitive::AddPlane(vertices, normals, glm::rotate(ninetyDegrees * 2, glm::vec3(0, 1, 0)));
    PlanePrimitive::AddPlane(vertices, normals, glm::rotate(ninetyDegrees * 3, glm::vec3(0, 1, 0)));

    PlanePrimitive::AddPlane(vertices, normals, glm::rotate(ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));
    PlanePrimitive::AddPlane(vertices, normals, glm::rotate(-ninetyDegrees, glm::vec3(0, 0, 1)) * glm::rotate(ninetyDegrees, glm::vec3(0, 1, 0)));

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
