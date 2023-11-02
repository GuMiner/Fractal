#include "Telemetry/Logger.h"
#include "Model.h"

#include <igl/readOFF.h>

// TODO allow for different model types
Eigen::MatrixXd V;
Eigen::MatrixXi F;

Model::Model() {
}

bool Model::Init(ShaderFactory* shaderFactory) {
    if (!shaderFactory->CreateShaderProgram("Config/Shaders/juliaFractal", &modelProgram))
    {
        Logger::LogError("Failed to load the model rendering shader; cannot continue.");
        return false;
    }

    igl::readOFF("Config/libigl-bunny.off", V, F);
    return true;
}