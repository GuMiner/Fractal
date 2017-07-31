 #include "JuliaFractalExtension.h"
// #include "BasicTerrainExtension.h"
#include "ShadedTerrainExtension.h"
#include "Terrain.h"

Terrain::Terrain()
{
}

bool Terrain::Init(ShaderFactory* shaderFactory, Viewer* viewer)
{
     backgroundRenderer = new BackgroundQuadRenderer(new JuliaFractalExtension(viewer));
     return backgroundRenderer->Init(shaderFactory, "juliaFractal");

    // backgroundRenderer = new BackgroundQuadRenderer(new BasicTerrainExtension(viewer));
    // return backgroundRenderer->Init(shaderFactory, "simpleTerrain");

    // backgroundRenderer = new BackgroundQuadRenderer(new BasicTerrainExtension(viewer));
    // return backgroundRenderer->Init(shaderFactory, "shadedTerrain");
}

void Terrain::Render(float gameTime)
{
    backgroundRenderer->Render(gameTime);
}

Terrain::~Terrain()
{
    delete backgroundRenderer;
}
