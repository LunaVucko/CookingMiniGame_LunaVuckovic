#include "PlayState.h"
#include "StoveState.h"
#include "MenuState.h"
#include <iostream>

PlayState::PlayState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/game_photo.png"))
    {
        std::cout << "Failed to load kitchen background\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);

    stoveArea = sf::FloatRect({ 320.f, 0.f }, { 320.f, 720.f }); //position of where to click the stove in the full kitchen layout
}

void PlayState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                static_cast<float>(mouse->position.x),
                static_cast<float>(mouse->position.y)
            );

            if (stoveArea.contains(mousePos))
            {
                manager.setState(std::make_unique<StoveState>(manager));
            }
        }
    }

    //escape back to menu
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<MenuState>(manager, 960, 720));
        }
    }
}

void PlayState::update() {}

void PlayState::draw(sf::RenderWindow& window)
{
    window.draw(background);
}