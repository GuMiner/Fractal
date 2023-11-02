#pragma once
#include "GLCore/ShaderFactory.h"
#include "Model.h"

class Scene
{
    Model* testModel;

public:
    Scene();
    void Init(ShaderFactory* shaderFactory);
    void RenderScene();
};