#include "SinkState.h"
#include "PlayState.h"
#include <iostream>

SinkState::SinkState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/sink_layout.png")) // <= background
    {
        std::cout << "Failed to load sink texture\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);

    //spritesheet

    if (!ingredientsTexture.loadFromFile("Texture/spritesheet_V2.png")) // <= ingreients assets
    {
        std::cout << "Failed to load spritesheet\n";
    }

    // addiing ingreideints to the inventory

    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 0,0 }, { 605,560 }))); //carrot
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 605,0 }, { 605,560 }))); //parsnip
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 1210,0 }, { 605,560 }))); //chicken breast
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 1860, 0 }, { 605, 560 }))); //cerialic
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 2465, 0 }, { 605, 560 }))); //garlic
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 0, 560 }, { 605, 560 }))); //buillion
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 605, 560 }, { 605, 560 }))); //parsley


    // sink area (adjust if needed)
    sinkArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

}

void SinkState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

            //nextState = std::make_unique<PlayState>(manager);
        }
    }

    // inventory drag
    inventory.handleEvent(event);

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (sinkArea.contains(mousePos))
        {
            auto item = inventory.takeDraggedItem();
            if (item)
            {
                sinkIngredients.push_back(move(item));
            }
               
        }
    }
}

void SinkState::update()
{
    // Nothing yet
}

void SinkState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    // Inventory bar
    inventory.draw(window);

    // sink ingredients
    for (auto& ing : sinkIngredients)
    {
        window.draw(ing->sprite);
    }

    // debug
    sf::RectangleShape debug;
    debug.setPosition(sinkArea.position);
    debug.setSize(sinkArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);
}