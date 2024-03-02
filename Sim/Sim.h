#pragma once
// These are setup via vcpkg. Navigate to them to figure out *which* vcpkg instance.
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Telemetry/FpsCounter.h"
#include "ThreadProcessor.h"
#include "Random2DFiller.h"
#include "GLCore/OpenGl.h"
#include "GLCore/ImguiRenderer.h"
#include "GLCore/ShaderFactory.h"
#include "Scene.h"
#include "Network/Lobby.h"

class Sim {
	OpenGl* opengl;

	FpsCounter* fpsCounter;
	ThreadProcessor* threadProcessor;

	// Single-threaded variant
	Random2DFiller* filler;

	ShaderFactory* shaderFactory;
	ImguiRenderer* guiRenderer;

	// Game mode scenese
	Lobby* lobby;
	Scene* testScene;

	void SetupDiagnostics();
	void Update();
	void Render();

public:
	Sim();
	~Sim();

	bool Init();
	void Run();
};