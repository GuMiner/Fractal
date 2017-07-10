#include "JuliaFractalExtension.h"
#include "Terrain.h"


Terrain::Terrain()
{
}

bool Terrain::Init(ShaderFactory* shaderFactory, Viewer* viewer)
{
    backgroundRenderer = new BackgroundQuadRenderer(new JuliaFractalExtension(viewer));
    return backgroundRenderer->Init(shaderFactory, "juliaFractal");
}

void Terrain::Render()
{
    backgroundRenderer->Render();
}

Terrain::~Terrain()
{
    delete backgroundRenderer;
}
