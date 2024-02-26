#include <iostream>
#include "Input.h"

std::set<GlfwKey> Input::pressedKeys;
std::set<GlfwMouseButton> Input::pressedMouseButtons;
bool Input::hasFocus;

bool Input::mouseInit = false;
glm::vec2 Input::lastMousePos = glm::vec2(0, 0);
glm::vec2 Input::lastMouseDelta = glm::vec2(1, 1);

std::vector<IGlfwMouseHandler*> Input::additionalMouseHandlers;
std::vector<IGlfwKeyHandler*> Input::additionalKeyHandlers;

void Input::Setup(GLFWwindow* window, GraphicsConfig& config) {
    // Raw mouse motion with no visible cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // Call the resize callback initially to keep our viewport accurate.
    Input::GlfwWindowResizeCallbacks(window, config.width, config.height);

    glfwSetKeyCallback(window, Input::GlfwKeyCallback);
    glfwSetCharCallback(window, Input::GlfwCharCallback);
    glfwSetWindowCloseCallback(window, Input::GlfwWindowCloseCallbacks);
    glfwSetWindowFocusCallback(window, Input::GlfwWindowFocusCallbacks);
    glfwSetWindowSizeCallback(window, Input::GlfwWindowResizeCallbacks);
    glfwSetMouseButtonCallback(window, Input::GlfwMouseButtonCallbacks);
    glfwSetScrollCallback(window, Input::GlfwScrollCallback);
}

#pragma region Callbacks

void Input::GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        pressedKeys.insert(key);

        // Perform (rare) direct actions.
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
    }
    else if (action == GLFW_RELEASE) {
        pressedKeys.erase(key);
    }

    for (IGlfwKeyHandler* handler : additionalKeyHandlers) {
        handler->GlfwKeyCallback(window, key, scancode, action, mods);
    }
}

void Input::GlfwCharCallback(GLFWwindow* window, unsigned int character) {
    for (IGlfwKeyHandler* handler : additionalKeyHandlers) {
        handler->GlfwCharCallback(window, character);
    }
}

void Input::GlfwMouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        pressedMouseButtons.insert(button);
    }
    else if (action == GLFW_RELEASE) {
        pressedMouseButtons.erase(button);
    }

    for (IGlfwMouseHandler* handler : additionalMouseHandlers) {
        handler->GlfwMouseButtonCallbacks(window, button, action, mods);
    }
}

void Input::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    for (IGlfwMouseHandler* handler : additionalMouseHandlers) {
        handler->GlfwScrollCallback(window, xoffset, yoffset);
    }
}

void Input::GlfwWindowCloseCallbacks(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Input::GlfwWindowFocusCallbacks(GLFWwindow* window, int focused) {
    hasFocus = focused == GLFW_TRUE;
}

void Input::GlfwWindowResizeCallbacks(GLFWwindow* window, int width, int height) {
    // // Letterboxing is done at the top and bottom.
    // float necessaryWidth = (float)height * viewer->GetAspectRatio();
    // if (necessaryWidth > width)
    // {
    //     // Letterbox the top and the bottom of the screen so that the aspect ratio is met
    //     float effectiveHeight = (float)width / viewer->GetAspectRatio();
    //     float heightDelta = ((float)height - effectiveHeight) / 2.0f;
    //     glViewport(0, (int)heightDelta, (GLsizei)width, (GLsizei)effectiveHeight);
    // }
    // else {
    //     // Letterbox the left and the right so that the aspect ratio is met.
    //     float widthDelta = ((float)width - necessaryWidth) / 2.0f;
    //     glViewport((GLint)widthDelta, (GLint)0, (GLsizei)necessaryWidth, (GLsizei)height);
    // }
    // 
    // viewer->SetScreenSize(width, height);

    // TODO consider if letterboxing is necessary and nice.
}
#pragma endregion

// Returns true if a key was pressed, false otherwise.
bool Input::IsKeyPressed(GlfwKey keyId) {
    return pressedKeys.find(keyId) != pressedKeys.end();
}

bool Input::IsMouseButtonPressed(GlfwMouseButton mouseButton) {
    return pressedMouseButtons.find(mouseButton) != pressedMouseButtons.end();
}

void Input::UpdateMouseDelta(GLFWwindow* window) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    glm::vec2 currentMousePos = glm::vec2(xPos, yPos);
    if (!mouseInit) {
        mouseInit = true; // Avoid mouse motion when nothing is initialized
    }
    else {
        lastMouseDelta = currentMousePos - lastMousePos;
    }

    lastMousePos = currentMousePos;
}

glm::vec2 Input::GetMouseDelta() {
    return lastMouseDelta;
}

glm::vec2 Input::GetMousePos(GLFWwindow* window)
{
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return glm::vec2((float)xPos, (float)yPos);
}

void Input::AddMouseHandler(IGlfwMouseHandler* mouseHandler) {
    additionalMouseHandlers.push_back(mouseHandler);
}

void Input::AddKeyHandler(IGlfwKeyHandler* keyHandler) {
    additionalKeyHandlers.push_back(keyHandler);
}