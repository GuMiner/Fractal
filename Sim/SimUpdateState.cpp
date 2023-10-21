#include "SimUpdateState.h"

SimUpdateState::SimUpdateState() :
	focusPaused(false), escapePaused(false), isAlive(true)
{
}
bool SimUpdateState::IsPaused()
{
	return focusPaused || escapePaused;
}

bool SimUpdateState::ShouldQuit()
{
	return !isAlive;
}

void SimUpdateState::Update(sf::Event& event)
{
    if (event.type == sf::Event::Closed)
    {
        isAlive = false;
    }
    else if (event.type == sf::Event::LostFocus)
    {
        focusPaused = true;
    }
    else if (event.type == sf::Event::GainedFocus)
    {
        focusPaused = false;
    }
    else if (event.type == sf::Event::KeyReleased && 
        event.key.code == sf::Keyboard::Escape)
    {
        escapePaused = !escapePaused;
    }
}