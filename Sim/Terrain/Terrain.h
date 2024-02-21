#pragma once
#include <atomic>
#include <map>
#include <thread>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"
#include "../Data/Config/TerrainConfig.h"
#include "TerrainRenderer.h"
#include "TerrainModel.h"

class Terrain {
    TerrainConfig config;

    std::map<int, TerrainModel*> models;
    TerrainRenderer* renderer;
    std::thread* terrainLoader;

    // Scaling factors
    float xTileRatio;
    float yXRatio;
    float scaleZ;

    int lastCameraTileX;
    int lastCameraTileY;

    int GetTileIndex(int x, int y, int mipsLevel);

    TerrainModel* TryCacheTile(int x, int y, int mipsIndex, int* gpuCounter);
    TerrainModel* EnsureTileCached(int x, int y, int mipsIndex, int* gpuCounter);

    bool stopSync;
    void LoadTerrainAsync();

    bool WithinRegion(int x, int y, int maxDist);
public:
    Terrain();
    bool Init(ShaderFactory* shaderFactory);
    void Render(Camera* camera);

    ~Terrain();
};

