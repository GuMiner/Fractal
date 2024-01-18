#pragma once
#include <set>
#include <glm/vec2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class KeyboardInput
{
    // List of pressed keys.
    static std::set<sf::Keyboard::Key> pressedKeys;
    static std::set<sf::Mouse::Button> pressedMouseButtons;
    static glm::ivec2 mousePos;

public:
    static bool IsKeyPressed(sf::Keyboard::Key key);
    static bool IsMouseButtonPressed(sf::Mouse::Button mouseButton);
    static glm::ivec2 GetMousePos();
    static bool HandleEvent(sf::Event event);
};

