#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Viewer.h"
#include "OpenGlCapabilities.h"

class OpenGl
{
    GLFWwindow* window;

    OpenGlCapabilities capabilities;
public:
    OpenGl();
    bool Load(Viewer* viewer);
    void Unload();
    
    GLFWwindow* GetWindow();
    const OpenGlCapabilities& GetCapabilities() const;
    void DisplayFrame();
};

