#include "StoveState.h"
#include "PlayState.h"
#include <iostream>

StoveState::StoveState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/stove_layout.png")) // <= background
    {
        std::cout << "Failed to load stove texture\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);

    // Pot area (adjust if needed)
    potArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

}

void StoveState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    //inventory drag
    manager.inventory.handleEvent(event);

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

           // nextState = std::make_unique<PlayState>(manager);
        }
    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (potArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (item)
            { 
                potIngredients.push_back(move(item));
            }
                
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

    // Inventory bar
    manager.inventory.draw(window);

    // Pot ingredients
    for (auto& ing : potIngredients)
    {
        window.draw(ing->sprite);
    }

    // debug
    sf::RectangleShape debug;
    debug.setPosition(potArea.position);
    debug.setSize(potArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);
}