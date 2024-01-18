#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"
#include "Model.h"
#include "Background/Fractal.h"
#include "Background/Sky.h"

class Scene {
    Camera* camera;
    Model* testModel;

    Fractal* fractal;
    Sky* sky;

    void ClearScreen();

public:
    Scene();
    bool Init(ShaderFactory* shaderFactory);
    void Update(float currentTime);
    void RenderScene(float currentTime);
};