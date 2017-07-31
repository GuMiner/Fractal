#pragma once
#include "shaders\ShaderFactory.h"
#include "BackgroundQuadRenderer.h"
#include "Viewer.h"

class Terrain
{
    BackgroundQuadRenderer* backgroundRenderer;
    Viewer* viewer;

public:
    Terrain();
    bool Init(ShaderFactory* shaderFactory, Viewer* viewer);
    void Render(float gameTime);
    ~Terrain();
};

