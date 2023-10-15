#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Telemetry/FpsCounter.h"

class Sim
{
	// TODO: Move to a diagnostic layer for 2D simulations?
	sf::Texture simTexture;
	sf::Sprite simSprite;

	FpsCounter* fpsCounter;

	void SetupDiagnostics();
	void Update(float currentTime);
	void Render(sf::RenderWindow& window);

public:
	Sim();
	~Sim();

	void Init();
	void Run();
};