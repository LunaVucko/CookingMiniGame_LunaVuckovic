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

    
    // sink area (adjust if needed)
    sinkArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

    if (!potTexture.loadFromFile("Texture/pot_spritesheet.png"))
    {
        std::cout << "Failed to load pot texture\n";
    }

    pot = std::make_unique<Pot>(
        potTexture,
        sf::IntRect({ 0,0 }, { 605,560 }),      // empty
        sf::IntRect({ 605,0 }, { 605,560 })     // filled
    );

    pot->sprite.setPosition({ 100.f, 500.f });
    pot->sprite.setScale({ 0.3f, 0.3f });
}

void SinkState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    //inventory drag
    //manager.inventory.handleEvent(event);

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

            //nextState = std::make_unique<PlayState>(manager);
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

            // pick up pot
            if (pot && pot->sprite.getGlobalBounds().contains(mousePos))
            {
                pot->isDragging = true;
                pot->dragOffset = pot->sprite.getPosition() - mousePos;
            }
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        currentMousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        manager.inventory.setMousePosition(currentMousePos);
    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        // Drop the pot in the sink then fill it
        if (pot && pot->isDragging && sinkArea.contains(mousePos))
        {
            pot->isDragging = false;

            if (pot->state == PotState::Empty)
            {
                pot->state = PotState::Filled;
                pot->updateSprite();

                std::cout << "Pot filled with water!\n";
            }

            return;
        }

        if (pot && pot->isDragging && manager.inventory.contains(mousePos))
        {
            pot->isDragging = false;

            pot->sprite.setPosition({ 50.f, 20.f }); // fake slot

            std::cout << "Pot placed in inventory (temporary)\n";
        }



        /*if (sinkArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (item)
            {
                sinkIngredients.push_back(move(item));
            }
               
        }*/
    }
}

void SinkState::update()
{
    if (pot && pot->isDragging)
    {
        pot->sprite.setPosition(currentMousePos + pot->dragOffset);
    }
}

void SinkState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    //if (pot)
    if (pot)
        window.draw(pot->sprite);

    // manager.inventory bar
    manager.inventory.draw(window);

    // sink ingredients
/*
    for (auto& ing : sinkIngredients)
    {
        window.draw(ing->sprite);
    }
    */

    // debug
    sf::RectangleShape debug;
    debug.setPosition(sinkArea.position);
    debug.setSize(sinkArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);
}