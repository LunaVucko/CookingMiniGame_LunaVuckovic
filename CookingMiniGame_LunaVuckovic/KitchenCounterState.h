#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include "Tools.h"
#include <memory>
#include <vector>

class KitchenCounterState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
   // Inventory inventory; //top inventory <= ingredients

    // Cutting board
    sf::FloatRect cuttingBoardArea;
    std::vector<std::unique_ptr<Ingredient>> counterIngredients;

    // tools

    std::vector<Tools> toolItems;
    ToolType currentTool = ToolType::None;


    //spritesheet

   // sf::Texture ingredientsTexture;
    sf::Texture toolsTexture;

    //mouse position
    Ingredient* draggedCounterItem = nullptr;
    sf::Vector2f currentMousePos;

   // Ingredient* selectedCounterItem = nullptr;

public:
    KitchenCounterState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
    
};