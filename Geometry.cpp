#include "Instance.h"
#include "Geometry.h"

Geometry::Geometry()
    : uvVbo(GL_STATIC_DRAW), normalVbo(GL_STATIC_DRAW), positionVbo(GL_STATIC_DRAW),
      vertexCount(0), sentToOpenGl(false), isGenerated(false)
{
}

void Geometry::SetGeometryData(int width, int height, std::vector<unsigned char>& textureData,
    std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs)
{
    texture.SetImageData(width, height, textureData);
    positionVbo.vertices = vertices;
    normalVbo.vertices = normals;
    uvVbo.vertices = uvs;

    // We allow those who don't want to specify a full set of parameters to do so by defining defaults.
    this->vertexCount = (int)(glm::max(glm::max(uvVbo.vertices.size(), positionVbo.vertices.size()), normalVbo.vertices.size()));
    for (int i = (int)uvVbo.vertices.size(); i < this->vertexCount; i++)
    {
        uvVbo.vertices.push_back(glm::vec2(0.5f, 0.5f)); // Default: Texture center
    }

    for (int i = (int)normalVbo.vertices.size(); i < this->vertexCount; i++)
    {
        normalVbo.vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); // Default: X+
    }

    for (int i = (int)positionVbo.vertices.size(); i < this->vertexCount; i++)
    {
        positionVbo.vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f)); // Default: Center
    }
}

void Geometry::SetAsGenerated()
{
    isGenerated = true;
}

bool Geometry::CanSendToGpu() const
{
    return isGenerated && !sentToOpenGl;
}

void Geometry::SendToGpu(int activeTextureOffset, IPerformanceProfiler* profiler)
{
    if (profiler->HasFrameTransferHitLimit(vertexCount, texture.GetWidth() * texture.GetHeight()))
    {
        return;
    }

    // TODO: update the names so this is InitializeOpenGl(), because we can send in vertex data before calling these methods.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    texture.Initialize(activeTextureOffset);
    uvVbo.Initialize();
    normalVbo.Initialize();
    positionVbo.Initialize();

    texture.TransferToOpenGl();
    uvVbo.TransferToOpenGl();
    normalVbo.TransferToOpenGl();
    positionVbo.TransferToOpenGl();

    sentToOpenGl = true;
}

void Geometry::Render(std::vector<Instance*> instances, IStandardRenderer* standardRenderer, IPerformanceProfiler* profiler) const
{
    if (sentToOpenGl)
    {
        int texels = texture.GetWidth() * texture.GetHeight();
        standardRenderer->SwapToGeometry(vao, texture.activeTextureOffset, texture.textureId, vertexCount);
        for (const Instance* instance : instances)
        {
            // Stop rendering if we hit our rendering limit
            if (profiler->HasFrameRenderHitLimit(vertexCount, texels))
            {
                break;
            }

            standardRenderer->RenderInstance(instance);
        }
    }
}

Geometry::~Geometry()
{
    if (sentToOpenGl)
    {
        glDeleteVertexArrays(1, &vao);
    }
}
