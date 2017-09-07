#include "CubeShape.h"

CubeShape::CubeShape(float divisions)
    : divisions(divisions)
{
}

CoreGeometry CubeShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // TODO: Generate cube.

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
