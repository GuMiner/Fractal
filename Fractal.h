#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include "shaders\ShaderFactory.h"
#include "ImguiRenderer.h"
#include "LineRenderer.h"
#include "Viewer.h"
#include "FpsCounter.h"

class Fractal
{
    GLFWwindow* window;
    ShaderFactory shaderFactory;
    ImguiRenderer guiRenderer;
    Viewer viewer;

    FpsCounter fpsCounter;

    // Top-level display items.
    bool LoadCoreGlslGraphics();

    void HandleEvents(bool& focusPaused);
    void Update(float currentTime, float frameTime);
    void Render(glm::mat4& viewMatrix);

public:
    Fractal();

    bool Initialize();
    void Deinitialize();

    bool LoadGraphics();
    void UnloadGraphics();
    
    bool Run();
};