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

    sf::Texture sinkHoverTexture;
    sf::Texture stoveHoverTexture;
    sf::Texture counterHoverTexture;

    sf::Vector2f mousePos;

    bool mouseInitialized = false;

    //tutoial buton

    sf::Font font;
    sf::Text cookbookText;
    sf::RectangleShape cookbookButton;

    //Inventory inventory;
    //sf::Texture ingredientsTexture;

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