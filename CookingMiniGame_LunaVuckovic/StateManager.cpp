#include "StateManager.h"
#include <iostream>

StateManager::StateManager()
{
    // Load texture ONCE
    if (!ingredientsTexture.loadFromFile("Texture/spritesheet_V2.png"))
    {
        std::cout << "Failed to load spritesheet\n";
        return;
    }

    // Add items ONCE
    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 0,0 }, { 605,560 }), IngredientType::Carrot));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 605,0 }, { 605,560 }), IngredientType::Parsnip));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 1210,0 }, { 605,560 }), IngredientType::Chicken));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 1860,0 }, { 605,560 }), IngredientType::Cereliac));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 2465,0 }, { 605,560 }), IngredientType::Garlic));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 0,560 }, { 605,560 }), IngredientType::Buillion));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 605,560 }, { 605,560 }), IngredientType::Parsley));
}

void StateManager::setState(std::unique_ptr<GameState> newState)
{
    currentState = std::move(newState);
}

void StateManager::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (currentState)
        currentState->handleEvent(window, event);
}

void StateManager::update()
{
    if (currentState)
        currentState->update();
}

void StateManager::draw(sf::RenderWindow& window)
{
    if (currentState)
        currentState->draw(window);
}