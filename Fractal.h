#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>
#include "shaders\ShaderFactory.h"
#include "FpsCounter.h"
#include "ImguiRenderer.h"
#include "ObjectLoader.h"
#include "OpenGl.h"
#include "Viewer.h"

class Fractal
{
    OpenGl opengl;
    ShaderFactory shaderFactory;
    ImguiRenderer guiRenderer;
    Viewer viewer;

    ObjectLoader objectLoader;
    FpsCounter fpsCounter;

    void HandleEvents(bool& focusPaused);
    void Update(float currentTime, float frameTime);
    void Render(float currentTime, glm::mat4& viewMatrix);

public:
    Fractal();

    bool Initialize();
    void Deinitialize();

    bool LoadGraphics();
    void UnloadGraphics();
    
    bool Run();
};