#include "TorusShape.h"

TorusShape::TorusShape(float angularResolution, float innerRadius, float outerRadius)
    : angularResolution(angularResolution), innerRadius(innerRadius), outerRadius(outerRadius)
{
}

CoreGeometry TorusShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // TODO: Generate torus.

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
