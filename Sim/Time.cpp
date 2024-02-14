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