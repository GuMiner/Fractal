#pragma once
#include <igl/readOFF.h>
#include "../GLCore/ShaderFactory.h"
#include "../Camera.h"

class TerrainModel
{
    GLuint modelProgram;

    GLuint modelVao;
    GLuint positionVbo;
    GLuint normalVbo;
    GLuint indexVbo;

    Eigen::MatrixXf V;
    Eigen::MatrixXi F;
    Eigen::MatrixXf N;

public:
    TerrainModel();
    bool Init(ShaderFactory* shaderFactory);
    bool SendMesh();
    void Render(Camera* camera, float currentTime);
};