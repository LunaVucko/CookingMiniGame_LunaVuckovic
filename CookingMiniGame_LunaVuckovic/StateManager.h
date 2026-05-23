#pragma once
#include <memory>
#include "GameState.h"
#include "Ingredient.h"
#include "Inventory.h" 
#include "Pot.h"
#include "Jug.h"
#include "ResultState.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>
#include "Ladle.h"
#include "Bowl.h"

class StateManager
{
private:
    std::unique_ptr<GameState> currentState;

    std::unique_ptr<Item> createIngredient(sf::Texture& texture, sf::Vector2i wholePos, sf::Vector2i peeledPos, sf::Vector2i cutPos, IngredientType type);

public:

    Inventory inventory{ InventoryType::Top }; //shared inventory
    //std::vector<std::unique_ptr<Ingredient>> inventory;

    //GLOBAL TEXTURES

    //ingredients
    sf::Texture ingredientsTexture;

    //bowl
    sf::Texture bowlTexture;

    //ladle
    sf::Texture ladleTexture;


    //pot
    sf::Texture potTexture;

    //jug
    sf::Texture jugTexture;

    //sink
    sf::Texture sinkTexture;
    sf::Texture waterTexture;

    //ingedients in pot
    sf::Texture potIngredientsTexture;

    //stove state backgrounds

    sf::Texture stoveEmptyTexture;
    sf::Texture stovePotTexture;
    sf::Texture stoveFlameTexture;
    sf::Texture* currentTexture = nullptr;

    //plating texture bacground

    sf::Texture platingTexture;

    //music

    sf::Music menuMusic;
    sf::Music gameMusic;

    //sfx

    sf::SoundBuffer knobOnBuffer;
    sf::SoundBuffer knobOffBuffer;

    std::optional<sf::Sound> knobOnSound;
    std::optional<sf::Sound> knobOffSound;

    sf::SoundBuffer waterOnBuffer;
    sf::SoundBuffer gasOnBuffer;

    std::optional<sf::Sound> waterOnSound;
    std::optional<sf::Sound> gasOnSound;

    // sink state data
    bool potInInventory = false;
    bool jugInInventory = false;
    // stove state data 

    bool stoveHasPot = false; // checking is stove has pot
    bool stoveHeatOn = false;

    bool stovePotSpawned = false;

    std::unique_ptr<Pot> stovePot;

    int requiredIngredients = 7;

        //Pot* pot = nullptr;
    //std::vector<std::unique_ptr<Item>> stoveItems;
   // std::vector<CookingItem> stoveItems;

    //CookingItem* activeCookingItem = nullptr;

    //end game

    int soupScore = 0;
    bool soupFinished = false;

    // TIMER
    sf::Clock gameTimer;

   // float gameDuration = 10.f; // 10 seconds for testing
   // float timeRemaining = 10.f;
    float gameDuration = 241.f; // 4 minutes
   float timeRemaining = 241.f;
       
  // float gameDuration = 21.f; // 20 seconds
   //float timeRemaining = 21.f;
   

    sf::Font timerFont;
    sf::Text timerText{ timerFont };

    bool gameOver = false;

    bool timerStarted = false;

    bool showTimer = false;

    bool pauseTimer = false;

    float totalPausedTime = 0.f;
    float pausedAt = 0.f;
    bool wasPaused = false;

    float elapsedTime = 0.f;
    float lastFrameTime = 0.f;

    StateManager();


    void setState(std::unique_ptr<GameState> newState);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);

    void update();

    void draw(sf::RenderWindow& window);

    std::unique_ptr<Pot> createPot();

    std::unique_ptr<Jug> createJug();

    std::unique_ptr<Ladle> createLadle();

    std::unique_ptr<Bowl> createBowl();

    void addIngredientScore(CookState state);

    void setupCookingRects(CookingItem& cookingItem, IngredientType type);

    void setupCookingTimes(CookingItem& cookingItem, IngredientType type);

    void resetGame();

    void initAudio();

    void playMenuMusic();

    void playGameMusic();

    void loadStartingInventory();


};
