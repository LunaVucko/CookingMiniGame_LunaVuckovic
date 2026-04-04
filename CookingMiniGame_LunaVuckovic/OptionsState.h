#pragma once
#include "GameState.h"

class StateManager;

class OptionsState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

public:
    OptionsState(StateManager& manager) : manager(manager)
    {
        texture.loadFromFile("Texture/option_photo.png");
        background.setSize({ 960, 720 });
        background.setTexture(&texture);
    }

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override
    {
        if (event.is<sf::Event::KeyPressed>())
        {
            if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
            {
                manager.setState(std::make_unique<MenuState>(manager, 960, 720));
            }
        }
    }

    void update() override {}

    void draw(sf::RenderWindow& window) override
    {
        window.draw(background);
    }
};
