#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"

class StateManager
{
private:
    std::unique_ptr<GameState> currentState;

public:


    std::vector<std::unique_ptr<Ingredient>> inventory;
    sf::Texture ingredientsTexture;


    void setState(std::unique_ptr<GameState> newState)
    {
        currentState = std::move(newState);
    }

    void handleEvent(sf::RenderWindow& window, const sf::Event& event)
    {
        if (currentState)
            currentState->handleEvent(window, event);
    }

    void update()
    {
        if (currentState)
            currentState->update();
    }

    void draw(sf::RenderWindow& window)
    {
        if (currentState)
            currentState->draw(window);
    }
};
