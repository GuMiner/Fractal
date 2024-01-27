#pragma once
#include <SFML\Window.hpp>

class SimUpdateState
{
	bool focusPaused;
	bool escapePaused;
	bool isAlive;
	bool isCaptureRequested;
public:
	SimUpdateState();
	bool IsFocusPaused();
	bool IsEscapePaused();
	bool IsCaptureRequested();
	bool ShouldQuit();
	void Update(sf::Event& event);
};