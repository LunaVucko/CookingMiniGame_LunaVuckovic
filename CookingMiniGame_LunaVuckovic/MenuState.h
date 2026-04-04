#pragma once
#include "GameState.h"
#include "MainMenu.h"
#include <memory>

class StateManager;

class MenuState : public GameState
{
private:
    MainMenu menu;
    sf::RectangleShape background;
    sf::Texture texture;

    StateManager& manager;

public:
    MenuState(StateManager& manager, float width, float height)
        : menu(width, height), manager(manager)
    {
        texture.loadFromFile("Texture/background.png");
        background.setSize({ width, height });
        background.setTexture(&texture);
    }

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;

    void update() override {}

    void draw(sf::RenderWindow& window) override
    {
        window.draw(background);
        menu.draw(window);
    }
};