#include <glm\trigonometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Time.h"

Time* Time::GlobalTime;

Time::Time(): clock(), gameTime(0.0f), lastFrameTime(0.0f),
	lastFrameInterval(-1), oneDay(20.0f) {

}

void Time::Update(bool isPaused) {
	lastFrameInterval = RunTime() - lastFrameTime;
	lastFrameTime = RunTime();

	if (!isPaused) {
		gameTime += lastFrameInterval;
	}
}

float Time::RunTime() {
	return clock.getElapsedTime().asSeconds();
}

float Time::GameTime() {
	return gameTime;
}

float Time::LastFrameInterval() {
	return lastFrameInterval;
}

bool Time::IsDay() {
	float halfDay = oneDay / 2;
	int halfInterval = (int)(gameTime / halfDay);
	return halfInterval % 2 == 0;
}

int Time::Day() {
	return (int)(gameTime / oneDay);
}

// TODO actually move with time again
glm::vec3 Time::SunDirection() {
	float time = 4.0f; // GameTime();
	float halfDay = oneDay / 2;

	float sunAngle =
		time * glm::radians(180.0f) / halfDay;
	auto flat = glm::vec3(glm::cos(sunAngle), 0, glm::sin(sunAngle));

	// Adjust for standard solar angle, averaging summer/winter
	return glm::normalize(
		glm::rotateX(flat, -glm::radians(40.0f)));
}

float Time::SunIntensity() {
	float time = 4.0f; // GameTime();
	float halfDay = oneDay / 2;

	float sunAngle =
		time * glm::radians(180.0f) / halfDay;

	// TODO add a night cycle
	return std::abs(glm::sin(sunAngle));
}