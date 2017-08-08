#include "Geometry.h"

Geometry::Geometry()
    : colorVbo(GL_STATIC_DRAW), normalVbo(GL_STATIC_DRAW), positionVbo(GL_STATIC_DRAW),
      vertexCount(0), sentToOpenGl(false), instances()
{
}

void Geometry::Render(IStandardRenderer * standardRenderer)
{
    if (sentToOpenGl)
    {
        standardRenderer->SwapToGeometry(vao, texture.activeTextureOffset, texture.textureId, vertexCount);
        for (const Instance& instance : instances)
        {
            standardRenderer->RenderInstance(instance);
        }
    }
}

Geometry::~Geometry()
{
}
