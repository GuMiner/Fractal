#include "ShadedTerrainExtension.h"

ShadedTerrainExtension::ShadedTerrainExtension(Viewer * viewer)
    : BasicTerrainExtension(viewer)
{
}

ShadedTerrainExtension::~ShadedTerrainExtension()
{
}

bool ShadedTerrainExtension::Init(GLuint programId)
{
    if (!BasicTerrainExtension::Init(programId))
    {
        return false;
    }

    return true;
}

void ShadedTerrainExtension::Render()
{
    BasicTerrainExtension::Render();
}
