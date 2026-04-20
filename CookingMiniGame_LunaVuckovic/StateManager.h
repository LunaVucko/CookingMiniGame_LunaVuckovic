#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"
#include "Inventory.h" 

class StateManager
{
private:
    std::unique_ptr<GameState> currentState;

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
