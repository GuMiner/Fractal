#pragma once
#include "GLCore/ShaderFactory.h"
#include "Camera.h"
#include "Model.h"
#include "Data/SaveFile.h"
#include "Terrain/Terrain.h"

#include "Terrain/InfiniGrid.h"
#include "Background/Sky.h"
#include "Background/Clouds.h"

// TODO move to a debug area
#include "Background/Fractal.h"
#include "Telemetry/Compass.h"

class Scene {
    SaveFile* save;

    Camera* camera;
    Model* testModel;

    Terrain* testTerrain;

    InfiniGrid* grid;
    Clouds* clouds;
    Sky* sky;

    Fractal* fractal;
    Compass* compass;

    void ClearScreen();

public:
    Scene();
    bool Init(ShaderFactory* shaderFactory);
    void Update();
    void RenderScene();

    ~Scene();
};