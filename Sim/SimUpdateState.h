#pragma once
#include <SFML\Window.hpp>

class SimUpdateState
{
	bool focusPaused;
	bool escapePaused;
	bool isAlive;
public:
	SimUpdateState();
	bool IsPaused();
	bool ShouldQuit();
	void Update(sf::Event& event);
};