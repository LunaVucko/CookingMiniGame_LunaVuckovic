#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"
#include "Inventory.h" 
#include "Pot.h"

class StateManager
{
private:
    std::unique_ptr<GameState> currentState;

    std::unique_ptr<Item> createIngredient(sf::Texture& texture, sf::Vector2i wholePos, sf::Vector2i peeledPos, sf::Vector2i cutPos, IngredientType type);

public:

    Inventory inventory{ InventoryType::Top }; //shared inventory
    //std::vector<std::unique_ptr<Ingredient>> inventory;

    //GLOBAL TEXTURES

    //ingredients
    sf::Texture ingredientsTexture;


    //pot
    sf::Texture potTexture;

    sf::Texture waterTexture;

    bool hasPot = false;


    StateManager();


    void setState(std::unique_ptr<GameState> newState);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void update();

    void draw(sf::RenderWindow& window);

    std::unique_ptr<Pot> createPot();
};
