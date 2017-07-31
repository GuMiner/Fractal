#pragma once
#include "IBackgroundRendererExtension.h"
#include "Viewer.h"

class JuliaFractalExtension : public IBackgroundRendererExtension
{
    GLuint cLocation;
    GLuint timeLocation;
    GLuint fractalGradientLocation;
    GLuint maxIterationsLocation;

    GLuint fractalGradientTextureId;

    int maxIterations;

    Viewer* viewer;

public:
    JuliaFractalExtension(Viewer* viewer);
    virtual ~JuliaFractalExtension() override;

    virtual bool Init(GLuint programId) override;
    virtual void Render(float gameTime) override;
};

