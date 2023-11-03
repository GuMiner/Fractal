#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"
#include "Model.h"

class Scene
{
    Camera* camera;
    Model* testModel;

public:
    Scene();
    bool Init(ShaderFactory* shaderFactory);
    void RenderScene(float currentTime);
};