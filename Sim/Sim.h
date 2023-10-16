#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Telemetry/FpsCounter.h"
#include "ThreadProcessor.h"
#include "Random2DFiller.h"

class Sim
{
	// TODO: Move to a diagnostic layer for 2D simulations?
	sf::Texture simTexture;
	sf::Sprite simSprite;

	FpsCounter* fpsCounter;
	ThreadProcessor* threadProcessor;

	// Single-threaded variant
	Random2DFiller* filler;

	void SetupDiagnostics();
	void Update(float currentTime);
	void Render(sf::RenderWindow& window);

public:
	Sim();
	~Sim();

	void Init();
	void Run();
};