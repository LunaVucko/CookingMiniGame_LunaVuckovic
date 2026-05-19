#pragma once

#include "GameState.h"
#include "StateManager.h"
#include "Pot.h"
#include "Ladle.h"
#include "Bowl.h"

class PlatingState : public GameState
{
private:

    StateManager& manager;

    sf::RectangleShape background;

    std::unique_ptr<Pot> pot;
    std::unique_ptr<Ladle> ladle;
    std::unique_ptr<Bowl> bowl;

    sf::Vector2f currentMousePos;

public:

    PlatingState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window,
        const sf::Event& event) override;

    void update() override;

    void draw(sf::RenderWindow& window) override;
};