#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Viewer.h"
#include "OpenGlCapabilities.h"
#include "OpenGlPerformanceProfiler.h"

class OpenGl
{
    GLFWwindow* window;

    OpenGlCapabilities capabilities;
    OpenGlPerformanceProfiler performanceProfiler;
public:
    OpenGl();
    bool Load(Viewer* viewer);
    void Unload();
    
    GLFWwindow* GetWindow();
    const OpenGlCapabilities& GetCapabilities() const;

    void PerformPerformanceAnalysis();
    void DisplayFrame();
};

