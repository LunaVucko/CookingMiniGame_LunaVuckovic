#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include "Pot.h"
#include <memory>
#include <vector>

class StoveState : public GameState
{
private:
    sf::RectangleShape background;
    //sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
    //Inventory inventory;

    // Pot
    sf::FloatRect potArea;
    sf::FloatRect knobArea;
   // std::vector<std::unique_ptr<Ingredient>> potIngredients;
   // std::vector<std::unique_poptr<Item>> potItems;
 //   std::unique_ptr<Pot> stovePot;
    //bool hasPotOnStove = false;
    //bool heatOn = false;
   // Pot* pot = nullptr;


public:
    StoveState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};