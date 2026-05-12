#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"
#include "Inventory.h" 
#include "Pot.h"
#include "Jug.h"

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

    //jug
    sf::Texture jugTexture;

    //sink
    sf::Texture sinkTexture;
    sf::Texture waterTexture;

    //ingedients in pot
    sf::Texture potIngredientsTexture;

    //stove state backgrounds

    sf::Texture stoveEmptyTexture;
    sf::Texture stovePotTexture;
    sf::Texture stoveFlameTexture;
    sf::Texture* currentTexture = nullptr;

    // sink state data
    bool potInInventory = false;
    bool jugInInventory = false;
    // stove state data 

    bool stoveHasPot = false; // checking is stove has pot
    bool stoveHeatOn = false;

        std::unique_ptr<Pot> stovePot;
    //std::vector<std::unique_ptr<Item>> stoveItems;
   // std::vector<CookingItem> stoveItems;

    //CookingItem* activeCookingItem = nullptr;


    StateManager();


    void setState(std::unique_ptr<GameState> newState);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void update();

    void draw(sf::RenderWindow& window);

    std::unique_ptr<Pot> createPot();

    std::unique_ptr<Jug> createJug();

    void setupCookingRects(CookingItem& cookingItem, IngredientType type);
};
