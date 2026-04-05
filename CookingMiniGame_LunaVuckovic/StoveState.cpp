#include "StoveState.h"
#include "PlayState.h"
#include <iostream>

StoveState::StoveState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/stove_layout.png"))
    {
        std::cout << "Failed to load stove texture\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);

}

void StoveState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        if (event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
        {
            // Escape to go back to PlayState (full kitchen layout view)
            manager.setState(std::make_unique<PlayState>(manager));
        }
    }
}

void StoveState::update()
{
    // Nothing yet
}

void StoveState::draw(sf::RenderWindow& window)
{
    window.draw(background);
}