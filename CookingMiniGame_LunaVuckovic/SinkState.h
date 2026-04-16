#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include <memory>
#include <vector>

class SinkState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
    std::vector<std::unique_ptr<Ingredient>> inventory;
    sf::RectangleShape inventoryBar;
    std::vector<sf::RectangleShape> slots;

    // Sink
    sf::FloatRect sinkArea;
    std::vector<std::unique_ptr<Ingredient>> sinkIngredients;

    //spritesheet

    sf::Texture ingredientsTexture;


public:
    SinkState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};