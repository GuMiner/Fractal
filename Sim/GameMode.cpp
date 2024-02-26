#include "Input/Input.h"
#include "GameMode.h"

GameMode* GameMode::State;

GameMode::GameMode(): wireframe(false), wireframePressed(false),
	IsPaused(false), pausePressed(false),
	IsCaptureRequested(false), WasCaptured(false) {

}

void GameMode::HandlePauseChange(GLFWwindow* window) {
	if (IsPaused) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}


void GameMode::Update(GLFWwindow* window) {
	// Handle paused
	if (Input::IsKeyPressed(GLFW_KEY_P) && !pausePressed) {
		// Transition to paused
		IsPaused = !IsPaused;
		pausePressed = true;

		HandlePauseChange(window);
	} else if (pausePressed) {
		pausePressed = false;
	}

	// Handle captured
	if (Input::IsKeyPressed(GLFW_KEY_C) && !IsCaptureRequested) {
		WasCaptured = false;
		IsCaptureRequested = true;
	}
	else if (IsCaptureRequested) {
		WasCaptured = false; 
		IsCaptureRequested = false;
	}

	if (Input::IsKeyPressed(GLFW_KEY_R) && !wireframePressed) {
		wireframe = !wireframe;
		wireframePressed = true;

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	}
	else if (wireframePressed) {
		wireframePressed = false;
	}
}