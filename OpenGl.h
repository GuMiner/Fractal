#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IPerformanceProfiler.h"
#include "OpenGlCapabilities.h"
#include "OpenGlPerformanceProfiler.h"
#include "Viewer.h"

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
    IPerformanceProfiler* GetPerformanceProfiler();
    void DisplayFrame();
};

