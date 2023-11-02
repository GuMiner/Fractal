#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Scene.h"
#include "Telemetry/FpsCounter.h"
#include "ThreadProcessor.h"
#include "Random2DFiller.h"
#include "SimUpdateState.h"
#include "GLCore/ShaderFactory.h"

class Sim
{
	// TODO: Move to a diagnostic layer for 2D simulations?
	sf::Texture simTexture;
	sf::Sprite simSprite;

	FpsCounter* fpsCounter;
	ThreadProcessor* threadProcessor;

	// Single-threaded variant
	Random2DFiller* filler;

	// Will be moved elsewhere, just testing integration
	GLuint testProgram;
	ShaderFactory* shaderFactory;

	Scene* testScene;

	void SetupDiagnostics();
	void UpdatePerspective(unsigned int width, unsigned int height);
	void HandleEvents(sf::RenderWindow& window, SimUpdateState& state);
	void Update(float currentTime);
	void Render(sf::RenderWindow& window, float currentTime);

public:
	Sim();
	~Sim();

	bool Init();
	void Run();
};