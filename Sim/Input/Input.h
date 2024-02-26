#pragma once
#include <set>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "IGlfwMouseHandler.h"
#include "IGlfwKeyHandler.h"
#include "../Data/Config/Config.h"

typedef int GlfwKey;
typedef int GlfwMouseButton;

class Input
{
    // List of pressed keys.
    static std::set<GlfwKey> pressedKeys;
    static std::set<GlfwMouseButton> pressedMouseButtons;
    static bool hasFocus;

    static bool mouseInit;
    static glm::vec2 lastMousePos;
    static glm::vec2 lastMouseDelta;

    // Additional external callbacks
    static std::vector<IGlfwMouseHandler*> additionalMouseHandlers;
    static std::vector<IGlfwKeyHandler*> additionalKeyHandlers;

    // Handles GLFW key callbacks.
    static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void GlfwCharCallback(GLFWwindow* window, unsigned int character);

    // Handles GLFW mouse callbacs
    static void GlfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
    static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Handles GLFW window callbacks.
    static void GlfwWindowCloseCallbacks(GLFWwindow* window);
    static void GlfwWindowFocusCallbacks(GLFWwindow* window, int focused);
    static void GlfwWindowResizeCallbacks(GLFWwindow* window, int width, int height);

public:
    static void Setup(GLFWwindow* window, GraphicsConfig& config);
    static bool IsKeyPressed(GlfwKey key);
    static bool IsMouseButtonPressed(GlfwMouseButton mouseButton);
    static void UpdateMouseDelta(GLFWwindow* window);
    static glm::vec2 GetMouseDelta();
    static glm::vec2 GetMousePos(GLFWwindow* window);

    static void AddMouseHandler(IGlfwMouseHandler* mouseHandler);
    static void AddKeyHandler(IGlfwKeyHandler* keyHandler);
};

