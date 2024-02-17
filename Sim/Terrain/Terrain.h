#pragma once
#include <map>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "../Data/Config/TerrainConfig.h"
#include "TerrainRenderer.h"
#include "TerrainModel.h"

class Terrain {
    TerrainConfig config;

    std::map<int, TerrainModel*> models;
    TerrainRenderer* renderer;

    int GetTileIndex(int x, int y, int mipsLevel);

public:
    Terrain();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);
};

