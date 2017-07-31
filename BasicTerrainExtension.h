#pragma once
#include "IBackgroundRendererExtension.h"
#include "Viewer.h"

class BasicTerrainExtension : public IBackgroundRendererExtension
{
    GLuint cameraPositionLocation;
    GLuint lookAtLocation;
    GLuint upLocation;
    GLuint aspectRatioLocation;
    GLuint fovYLocation;

    Viewer* viewer;

public:
    BasicTerrainExtension(Viewer* viewer);
    virtual ~BasicTerrainExtension() override;

    virtual bool Init(GLuint programId) override;
    virtual void Render(float gameTime) override;
};

