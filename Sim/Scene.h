#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"
#include "Model.h"
#include "Background/Fractal.h"

class Scene {
    Camera* camera;
    Model* testModel;
    Fractal* fractal;

    void ClearScreen();

public:
    Scene();
    bool Init(ShaderFactory* shaderFactory);
    void RenderScene(float currentTime);
};