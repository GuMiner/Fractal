#include "SphereShape.h"

SphereShape::SphereShape(int subdivisions)
    : subdivisions(subdivisions)
{
}

CoreGeometry SphereShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // TODO: Add our basic tetrahedron, with points at r=1
    // 

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
