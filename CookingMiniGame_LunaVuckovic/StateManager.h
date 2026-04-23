#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"
#include "Inventory.h" 

class StateManager
{
private:
    std::unique_ptr<GameState> currentState;

    std::unique_ptr<Ingredient> createIngredient(sf::Texture& texture, sf::Vector2i wholePos, sf::Vector2i peeledPos, sf::Vector2i cutPos, IngredientType type);

public:

    Inventory inventory{ InventoryType::Top }; //shared inventory
    //std::vector<std::unique_ptr<Ingredient>> inventory;
    sf::Texture ingredientsTexture;

    StateManager();


    void setState(std::unique_ptr<GameState> newState);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void update();

    void draw(sf::RenderWindow& window);
};
