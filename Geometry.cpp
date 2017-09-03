#include "Instance.h"
#include "Geometry.h"

Geometry::Geometry()
    : colorVbo(GL_STATIC_DRAW), normalVbo(GL_STATIC_DRAW), positionVbo(GL_STATIC_DRAW),
      vertexCount(0), sentToOpenGl(false), isGenerated(false)
{
}

void Geometry::SetAsGenerated()
{
    isGenerated = true;
}

bool Geometry::CanSendToGpu() const
{
    return isGenerated && !sentToOpenGl;
}

void Geometry::SendToGpu()
{
    texture.TransferToOpenGl();
    colorVbo.TransferToOpenGl();
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
}
