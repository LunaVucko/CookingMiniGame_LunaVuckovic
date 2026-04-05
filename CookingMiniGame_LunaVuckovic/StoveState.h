#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"

class StoveState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

public:
    StoveState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};