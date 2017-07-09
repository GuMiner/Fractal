#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Viewer.h"

class OpenGl
{
    GLFWwindow* window;
public:
    OpenGl();
    bool Load(Viewer* viewer);
    void Unload();
    
    GLFWwindow* GetWindow();
    void DisplayFrame();
};

