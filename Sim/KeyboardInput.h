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
    static glm::ivec2 centralMousePos;
    static glm::ivec2 lastMouseDelta;

public:
    static bool IsKeyPressed(sf::Keyboard::Key key);
    static bool IsMouseButtonPressed(sf::Mouse::Button mouseButton);
    static void ResetMouseDelta();
    static glm::ivec2 GetMouseDelta();
    static void SetMouseCenter(glm::ivec2 center);
    static bool HandleEvent(sf::Event event);
};

