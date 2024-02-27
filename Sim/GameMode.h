#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GameMode {
    bool wireframe;
    bool wireframePressed;

    bool pausePressed;

    void HandlePauseChange(GLFWwindow* window);

public:
    static GameMode* State;

    GameMode();
    void Update(GLFWwindow* window);

    bool IsPaused;
    bool IsCaptureRequested;
    bool WasCaptured;

    bool IsDialogMode;
};

