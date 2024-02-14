#pragma once
#include <SFML\System.hpp>

class Time {
	sf::Clock clock;

	float gameTime;
	
	float lastFrameTime;
	float lastFrameInterval;

	// TODO move to config
	float oneDay;
public:
	static Time* GlobalTime;

	Time();

	float RunTime();
	float GameTime();
	float LastFrameInterval();

	void Update(bool isPaused);

	bool IsDay();
	int Day();
};

