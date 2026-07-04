#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include <memory>
#include <vector>
#include "Pot.h"
#include "Jug.h"
#include "sinkTutorialStage.h"

class SinkState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Sink
    sf::FloatRect sinkArea;

   // std::unique_ptr<Pot> pot;

    std::unique_ptr<Jug> jug;

    //mouse 
    sf::Vector2f currentMousePos;

    // knob area
    sf::FloatRect knobArea;

    //tutoial buton

    sf::Font font;
    sf::Text cookbookText;
    sf::RectangleShape cookbookButton;


    // Tutorial knob indicator
    sf::RectangleShape knobIndicator;
    float indicatorRotation = 0.f;
  //  bool showKnobIndicator = true;

    //tutorial dragging jug

    sf::RectangleShape dragArrow;

   // bool showDragArrow = false;

    float hintAnimTime = 0.f;

    sf::Vector2f arrowStart;
    sf::Vector2f arrowEnd;

    SinkTutorialStage sinkTutorialStage = SinkTutorialStage::TurnWaterOn;

    // water state
    bool waterOn = false;

    sf::Texture* currentBackgroundTexture;

    // mouse action tracking
    bool isTurningKnob = false;
    std::vector<sf::Vector2f> knobTrail;

    Item* pendingDragItem = nullptr;

    sf::Vector2f pressStartPos;

    bool mouseHeld = false;



public:
    SinkState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};