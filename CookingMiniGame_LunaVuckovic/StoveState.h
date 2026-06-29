#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include "Pot.h"
#include <memory>
#include <vector>
#include "stoveTutorialStage.h"

struct SmokeParticle
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime = 0.f;
};

class StoveState : public GameState
{
private:
    sf::RectangleShape background;
    //sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
    //Inventory inventory;


    // Tutorial indicator

    StoveTutorialStage stoveTutorialStage = StoveTutorialStage::DragPot;

    float hintAnimTime = 0.f;

    // Pot arrow
    sf::RectangleShape potArrow;
    sf::Vector2f potArrowStart;
    sf::Vector2f potArrowEnd;

    // Ingredient arrow
    sf::RectangleShape ingredientArrow;
    sf::Vector2f ingredientArrowStart;
    sf::Vector2f ingredientArrowEnd;

    //knob indicator

    sf::RectangleShape knobIndicator;
    float indicatorRotation = 0.f;


    // Pot
    sf::FloatRect potArea;
    sf::FloatRect knobArea;

    //std::unique_ptr<Pot> pot;

    //mouse 
    sf::Vector2f currentMousePos;

    //animation

    std::vector<SmokeParticle> smokeParticles;

    void spawnSmoke(sf::Vector2f position, sf::Color color);

   // std::vector<std::unique_ptr<Ingredient>> potIngredients;
   // std::vector<std::unique_poptr<Item>> potItems;
 //   std::unique_ptr<Pot> stovePot;
    //bool hasPotOnStove = false;
    //bool heatOn = false;
   // Pot* pot = nullptr;

    bool potHasWater = false;

    bool waterInitialized = false;

    CookingItem waterItem;

    sf::IntRect waterRect = sf::IntRect({ 615, 2250 }, { 605,560 });
    sf::IntRect waterCookedRect = sf::IntRect({ 1860, 2250 }, { 605,560 });


public:
    StoveState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;
};