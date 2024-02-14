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

glm::vec3 Time::SunDirection() {
	float halfDay = oneDay / 2;
	float quarterDay = halfDay / 2;

	float sunAngle =
		(gameTime - quarterDay) * glm::radians(180.0f) / halfDay;
	auto flat = glm::vec3(glm::cos(sunAngle), 0, glm::sin(sunAngle));

	// Adjust for standard solar angle, averaging summer/winter
	return glm::normalize(
		glm::rotateX(flat, -glm::radians(40.0f)));
}

float Time::SunIntensity() {
	float halfDay = oneDay / 2;
	float quarterDay = halfDay / 2;

	float sunAngle =
		(gameTime - quarterDay) * glm::radians(180.0f) / halfDay;

	return std::min(0.0f, glm::sin(sunAngle));
}