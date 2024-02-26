#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Data/Config/Config.h"
#include "OpenGlCapabilities.h"

class OpenGl {
    GLFWwindow* window;

    OpenGlCapabilities capabilities;
public:
    OpenGl();
    bool Load(GraphicsConfig& config);
    void Unload();

    GLFWwindow* GetWindow();
    const OpenGlCapabilities& GetCapabilities() const;
    void DisplayFrame();
};

