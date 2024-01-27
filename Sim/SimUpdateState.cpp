#include "SimUpdateState.h"

SimUpdateState::SimUpdateState() :
	focusPaused(false), escapePaused(false), isAlive(true), isCaptureRequested(false)
{
}

bool SimUpdateState::IsFocusPaused() {
	return focusPaused;
}

bool SimUpdateState::IsEscapePaused() {
    return escapePaused;
}

bool SimUpdateState::IsCaptureRequested() {
    return isCaptureRequested;
}


bool SimUpdateState::ShouldQuit() {
	return !isAlive;
}

void SimUpdateState::Update(sf::Event& event)
{
    // Reset one-time states
    isCaptureRequested = false;

    if (event.type == sf::Event::Closed) {
        isAlive = false;
    }
    else if (event.type == sf::Event::LostFocus) {
        focusPaused = true;
    }
    else if (event.type == sf::Event::GainedFocus) {
        focusPaused = false;
    }
    else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Escape) {
            escapePaused = !escapePaused;
        }
        else if (event.key.code == sf::Keyboard::C) {
            isCaptureRequested = true;
        }
    }
}