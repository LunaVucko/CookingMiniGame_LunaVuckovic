#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "StateManager.h"
#include "Ingredient.h"
#include "Inventory.h"
#include "Tools.h"
#include <memory>
#include <vector>
#include <SFML/Audio.hpp>
#include "counterTutorialStage.h"

enum class CreatureMood
{
    Default,
    Happy,
    Angry,
    Scared,
    SideEye
};

class KitchenCounterState : public GameState
{
private:
    sf::RectangleShape background;
    sf::Texture texture;
    StateManager& manager;

    std::unique_ptr<GameState> nextState = nullptr;

    // Inventory
   // Inventory inventory; //top inventory <= ingredients

    // Creature
    sf::RectangleShape creature;
    CreatureMood creatureMood = CreatureMood::Default;

    sf::Clock creatureClock;
    float creatureDuration = 1.5f;

    // Cutting board
    sf::FloatRect cuttingBoardArea;
    std::vector<std::unique_ptr<Item>> counterItems;
   // std::vector<std::unique_ptr<Ingredient>> counterIngredients;

    // tools

    std::vector<Tools> toolItems;
    ToolType currentTool = ToolType::None;

    ///custom cursor

    sf::RectangleShape toolCursor;
    bool showToolCursor = false;

    //tutoial buton

    sf::Font font;
    sf::Text cookbookText;
    sf::RectangleShape cookbookButton;

    //turoial

    sf::RectangleShape tutorialArrow;

    sf::RectangleShape peelLine;
    sf::RectangleShape cutLine;

    sf::RectangleShape peelerFeedback;
    sf::RectangleShape knifeFeedback;

    CounterTutorialStage counterTutorialStage =
        CounterTutorialStage::DragToBoard;

    float hintAnimTime = 0.f;



    //spritesheet

   // sf::Texture ingredientsTexture;
    sf::Texture toolsTexture;

    //mouse position
    //Ingredient* draggedCounterItem = nullptr;
    sf::Vector2f currentMousePos;

    // Selected Ingredient
   Ingredient* selectedIngredient = nullptr;

   // Mouse Action system
   sf::Vector2f dragStart;
   sf::Vector2f dragEnd;
   bool isDraggingGesture = false;

   Item* pendingDragItem = nullptr;
   sf::Vector2f pressStartPos;
   bool mouseHeld = false;

   // animation
   bool isAnimating = false;

   ToolType animationTool = ToolType::None;

   sf::Clock animationClock;

   float animationDuration = 0.7f;

   // animated tool sprite
   std::optional<sf::Sprite> animationSprite;


   //sfx

   sf::SoundBuffer cutBuffer;
   sf::SoundBuffer peelBuffer;
  
   std::optional<sf::Sound> cutSound;
   std::optional<sf::Sound> peelSound;
 

public:
    KitchenCounterState(StateManager& manager);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event) override;
    void update() override;
    void draw(sf::RenderWindow& window) override;

    void setCreatureMood(CreatureMood mood);
    
};