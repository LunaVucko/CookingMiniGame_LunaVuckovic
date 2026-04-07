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

    // Inventory bar
    inventoryBar.setSize({ 960, 100 });
    inventoryBar.setPosition({ 0, 0 });
    inventoryBar.setFillColor(sf::Color(50, 50, 50, 200));

    // Add ingredients
    auto addIngredient = [&](const std::string& path)
        {
            auto ing = std::make_unique<Ingredient>(path);

            if (ing->isLoaded)
            {
                inventory.push_back(std::move(ing));
            }
        };

    addIngredient("Texture/parsnip.jpg");
    addIngredient("Texture/carrot.png");
    addIngredient("Texture/chicken.jpg");


    // Position ingredients
    float startX = 20.f;
    for (auto& ing : inventory)
    {
        ing->sprite.setPosition({ startX, 20.f });
        ing->sprite.setScale({ 0.5f, 0.5f });
        startX += 80.f;
    }

    // Pot area (adjust if needed)
    potArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

}

void StoveState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));
        }
    }

    // pressing the mouse starts the dragging
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                (float)mouse->position.x,
                (float)mouse->position.y
            );

            for (auto& ing : inventory)
            {
                if (ing->sprite.getGlobalBounds().contains(mousePos) &&
                    inventoryBar.getGlobalBounds().contains(mousePos))
                {
                    ing->isDragging = true;
                }
            }
        }
    }

    // when mooving the mouse the ingredients are being moved ( dragged )
    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        sf::Vector2f mousePos(
            (float)mouse->position.x,
            (float)mouse->position.y
        );

        for (auto& ing : inventory)
        {
            if (ing->isDragging)
            {
                ing->sprite.setPosition(mousePos);
            }
        }
    }

    // realasing the mouse drops the items
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos(
            (float)mouse->position.x,
            (float)mouse->position.y
        );

        for (size_t i = 0; i < inventory.size(); i++)
        {
            auto& ing = inventory[i];

            if (ing->isDragging)
            {
                if (potArea.contains(mousePos))
                {
                    // Move safely to pot
                    potIngredients.push_back(std::move(ing));

                    // Remove from inventory
                    inventory.erase(inventory.begin() + i);
                    break;
                }

                ing->isDragging = false;
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
    window.draw(inventoryBar);

    // Inventory items
    for (auto& ing : inventory)
    {
        window.draw(ing->sprite);
    }

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