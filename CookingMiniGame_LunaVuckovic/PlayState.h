#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Inventory.h"

class PlayState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    Inventory inventory;
    sf::Texture ingredientsTexture;

    sf::FloatRect stoveArea;
    sf::FloatRect kitchenCounterArea;
    sf::FloatRect sinkArea;

    std::unique_ptr<GameState> nextState = nullptr;

public:
    PlayState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};