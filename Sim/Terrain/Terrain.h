#pragma once
#include <map>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "TerrainRenderer.h"
#include "TerrainModel.h"

class Terrain
{
    std::map<int, TerrainModel*> models;
    TerrainRenderer* renderer;
public:
    Terrain();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);
};

