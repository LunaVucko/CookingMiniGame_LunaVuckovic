#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"

class StateManager;

enum class ResultType
{
    Score,
    TimeUp
};

class ResultState : public GameState
{
private:
    StateManager& manager;

    sf::Font font;

    sf::Text resultText;
    sf::Text scoreText;
    sf::Text continueText;

    sf::RectangleShape background;

    ResultType resultType;



public:
    ResultState(StateManager& manager, ResultType resultType);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;

    void update() override;

    void draw(sf::RenderWindow& window) override;
};