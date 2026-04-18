#include "PlayState.h"
#include "StoveState.h"
#include "SinkState.h"
#include "KitchenCounterState.h"
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

    //spritesheet

    if (!ingredientsTexture.loadFromFile("Texture/spritesheet_V2.png")) // <= ingreients assets
    {
        std::cout << "Failed to load spritesheet\n";
    }

    // addiing ingreideints  to the inventory

    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 0,0 }, { 605,560 }))); //carrot
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 605,0 }, { 605,560 }))); //parsnip
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 1210,0 }, { 605,560 }))); //chicken breast
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 1860, 0 }, { 605, 560 }))); //cerialic
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 2465, 0 }, { 605, 560 }))); //garlic
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 0, 560 }, { 605, 560 }))); //buillion
    inventory.addItem(std::make_unique<Ingredient>(ingredientsTexture, sf::IntRect({ 605, 560 }, { 605, 560 }))); //parsley

    float inventoryHeight = 100.f;

    stoveArea = sf::FloatRect({ 320.f, 0.f }, { 320.f, 720.f - inventoryHeight}); //position of where to click the stove in the full kitchen layout
    kitchenCounterArea = sf::FloatRect({ 640.f, 0.f }, { 320.f, 720.f - inventoryHeight}); //position of where to click the counter in the full kitchen layout
    sinkArea = sf::FloatRect({ 0.f, 0.f }, { 320.f, 720.f - inventoryHeight}); //position of where to click the sink in the full kitchen layout

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
            
            if (inventory.contains(mousePos))
                return;

            if (sinkArea.contains(mousePos))
            {
                nextState = std::make_unique<SinkState>(manager);
            }
            else if (stoveArea.contains(mousePos))
            {
                nextState = std::make_unique<StoveState>(manager);
            }
            else if (kitchenCounterArea.contains(mousePos))
            {
                nextState = std::make_unique<KitchenCounterState>(manager);
            }
        }
    }

    //escape back to menu
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            nextState = std::make_unique<MenuState>(manager, 960, 720);
        }
    }
}

void PlayState::update() 
{
    if (nextState)
    {
        manager.setState(std::move(nextState));
    }
}

void PlayState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    //inventory

    inventory.draw(window);

    
    sf::RectangleShape debugRect;

    //Sink (Left - blue)
    debugRect.setPosition({ 0.f, 0.f });
    debugRect.setSize({ 320.f, 720.f });
    debugRect.setFillColor(sf::Color(0, 0, 225, 100));
    window.draw(debugRect);

    //Stove (Middle - red)
    debugRect.setPosition({ 320.f, 0.f });
    debugRect.setFillColor(sf::Color(255, 0, 0, 100));
    window.draw(debugRect);


    //Counter (Right - green)
    debugRect.setPosition({ 640.f, 0.f });
    debugRect.setFillColor(sf::Color(0, 255, 0, 100));
    window.draw(debugRect);
    
}