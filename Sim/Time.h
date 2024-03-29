#pragma once
#include <glm/vec3.hpp>

class Time {
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

	glm::vec3 SunDirection();
	float SunIntensity();
};

