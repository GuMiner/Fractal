#include "KeyboardInput.h"
#include <iostream>

std::set<sf::Keyboard::Key> KeyboardInput::pressedKeys;
std::set<sf::Mouse::Button> KeyboardInput::pressedMouseButtons;
glm::ivec2 KeyboardInput::centralMousePos;
glm::ivec2 KeyboardInput::lastMouseDelta = glm::ivec2(1, 1);

// Returns true if a key was pressed, false otherwise.
bool KeyboardInput::IsKeyPressed(sf::Keyboard::Key keyId) {
    return pressedKeys.find(keyId) != pressedKeys.end();
}

bool KeyboardInput::IsMouseButtonPressed(sf::Mouse::Button mouseButton) {
    return pressedMouseButtons.find(mouseButton) != pressedMouseButtons.end();
}

glm::ivec2 KeyboardInput::GetMouseDelta() {
    return lastMouseDelta;
}

void KeyboardInput::ResetMouseDelta() {
    lastMouseDelta = glm::ivec2(sf::Mouse::getPosition().x - centralMousePos.x, sf::Mouse::getPosition().y - centralMousePos.y);
}

void KeyboardInput::SetMouseCenter(glm::ivec2 center) {
    centralMousePos = center;
}

bool KeyboardInput::HandleEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        pressedKeys.insert(event.key.code);
        return true;
    }
    else if (event.type == sf::Event::KeyReleased) {
        pressedKeys.erase(event.key.code);
        return true;
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        pressedMouseButtons.insert(event.mouseButton.button);
        return true;
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        pressedMouseButtons.erase(event.mouseButton.button);
        return true;
    }

    return false;
}