#include "CoreGeometry.h"



CoreGeometry::CoreGeometry()
    : normalVbo(GL_STATIC_DRAW), positionVbo(GL_STATIC_DRAW)
{
}

void CoreGeometry::SetGeometryData(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
{
    positionVbo.vertices = vertices;
    normalVbo.vertices = normals;
}

size_t CoreGeometry::GetMaxVertexCount() const
{
    return glm::max(normalVbo.vertices.size(), positionVbo.vertices.size());
}

void CoreGeometry::NormalizeVertexCount(int maxVertexCount)
{
    for (int i = (int)normalVbo.vertices.size(); i < maxVertexCount; i++)
    {
        normalVbo.vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // Default: X+
    }

    for (int i = (int)positionVbo.vertices.size(); i < maxVertexCount; i++)
    {
        positionVbo.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // Default: Center
    }
}

void CoreGeometry::SetupOpenGlBuffers()
{
    normalVbo.SetupOpenGlBuffers();
    positionVbo.SetupOpenGlBuffers();
}

void CoreGeometry::TransferToOpenGl()
{
    normalVbo.TransferToOpenGl();
    positionVbo.TransferToOpenGl();
}

void CoreGeometry::Deinitialize()
{
    normalVbo.Deinitialize();
    positionVbo.Deinitialize();
}
