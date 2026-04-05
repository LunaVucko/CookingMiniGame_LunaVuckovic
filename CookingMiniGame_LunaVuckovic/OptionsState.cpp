#include "OptionsState.h"
#include "MenuState.h"
#include <iostream>

OptionsState::OptionsState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/option_photo.png"))
    {
        std::cout << "Failed to load options background\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);
}

void OptionsState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{ // escape back to menu
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<MenuState>(manager, 960, 720));
        }
    }
}

void OptionsState::update()
{
    // nothing yet
}

void OptionsState::draw(sf::RenderWindow& window)
{
    window.draw(background);
}