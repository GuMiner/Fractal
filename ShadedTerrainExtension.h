#pragma once
#include "BasicTerrainExtension.h"
#include "Viewer.h"

class ShadedTerrainExtension : public BasicTerrainExtension
{
public:
    ShadedTerrainExtension(Viewer* viewer);
    virtual ~ShadedTerrainExtension() override;

    virtual bool Init(GLuint programId) override;
    virtual void Render(float gameTime) override;
};

