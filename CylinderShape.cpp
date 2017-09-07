#include "CylinderShape.h"

CylinderShape::CylinderShape(float angularResolution)
    : angularResolution(angularResolution), lowerRadius(1.0f), upperRadius(1.0f)
{
}

CylinderShape::CylinderShape(float angularResolution, float lowerRadius, float upperRadius)
    : angularResolution(angularResolution), lowerRadius(lowerRadius), upperRadius(upperRadius)
{
}

CoreGeometry CylinderShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // TODO: Generate cylinder.

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
