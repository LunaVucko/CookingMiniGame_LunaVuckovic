#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"

class ResultState : public GameState
{
private:
    StateManager& manager;

    sf::Font font;

    sf::Text resultText;
    sf::Text scoreText;
    sf::Text continueText;

    sf::RectangleShape background;

public:
    ResultState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;

    void update() override;

    void draw(sf::RenderWindow& window) override;
};