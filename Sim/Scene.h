#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"
#include "Model.h"
#include "Terrain/Terrain.h"
#include "Background/Sky.h"

// TODO move to a debug area
#include "Background/Fractal.h"
#include "Telemetry/Compass.h"

class Scene {
    Camera* camera;
    Model* testModel;
    Terrain* testTerrain;

    Sky* sky;

    Fractal* fractal;
    Compass* compass;

    void ClearScreen();

public:
    Scene();
    bool Init(ShaderFactory* shaderFactory);
    void Update();
    void RenderScene();
};