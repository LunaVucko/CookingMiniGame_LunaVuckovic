#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include <memory>
#include <vector>

class StoveState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
    //Inventory inventory;

    // Pot
    sf::FloatRect potArea;
    std::vector<std::unique_ptr<Ingredient>> potIngredients;

    //spritesheet

    //sf::Texture ingredientsTexture;


public:
    StoveState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};