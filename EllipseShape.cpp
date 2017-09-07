#include "EllipseShape.h"

EllipseShape::EllipseShape(float angularResolution, glm::vec3 radii)
    : angularResolution(angularResolution), radii(radii)
{
}

CoreGeometry EllipseShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // TODO: Generate ellipse.

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
