#include "StateManager.h"
#include <iostream>
#include "MenuState.h"


StateManager::StateManager()
{
    // Load texture ONCE

    //ingredients

    if (!ingredientsTexture.loadFromFile("Texture/spritesheet_V2.png"))
    {
        std::cout << "Failed to load spritesheet\n";
        return;
    }

    //pot

    if (!potTexture.loadFromFile("Texture/pot_spritesheet.png"))
    {
        std::cout << "Failed to load pot texture\n";
    }

    //jug

    if (!jugTexture.loadFromFile("Texture/tools_spritesheet.png"))
    {
        std::cout << "Failed to load tools texture\n";
    }

    //bowl


    if (!bowlTexture.loadFromFile("Texture/tools_spritesheet.png"))
    {
        std::cout << "Failed to load tools texture\n";
    }


    //ladl

    if (!ladleTexture.loadFromFile("Texture/tools_spritesheet.png"))
    {
        std::cout << "Failed to load tools texture\n";
    }

    // sink

    if (!sinkTexture.loadFromFile("Texture/sink_layout.png"))
    {
        std::cout << "Failed to load water texture\n";
    }


    if (!waterTexture.loadFromFile("Texture/sink_water.png"))
    {
        std::cout << "Failed to load water texture\n";
    }

    //roate hint 

    if (!rotateHintTexture.loadFromFile("Texture/feedback_knob.png"))
    {
        std::cout << "Failed to load knob feedback\n";
        return;
    }

    //drag hint 

    if (!arrowJugHintTexture.loadFromFile("Texture/feedback_arrow.png"))
    {
        std::cout << "Failed to load drag feedback\n";
        return;
    }

    //cut hint 

    if (!cutHintTexture.loadFromFile("Texture/cut_line.png"))
    {
        std::cout << "Failed to load cut feedback\n";
        return;
    }
    //peel hint 

    if (!peelHintTexture.loadFromFile("Texture/peel_line.png"))
    {
        std::cout << "Failed to load peel feedback\n";
        return;
    }
    //knfie hint 

    if (!knifeHintTexture.loadFromFile("Texture/knife_feedback.png"))
    {
        std::cout << "Failed to load drag feedback\n";
        return;
    }
    //peeler hint 

    if (!peelerHintTexture.loadFromFile("Texture/peeler_feedback_.png"))
    {
        std::cout << "Failed to load drag feedback\n";
        return;
    }

    //creature sprites

    if (!creatureDefaultTexture.loadFromFile("Texture/creature_default.png"))
        std::cout << "Failed creature default\n";

    if (!creatureHappyTexture.loadFromFile("Texture/creature_happy.png"))
        std::cout << "Failed creature happy\n";

    if (!creatureAngryTexture.loadFromFile("Texture/creature_angry.png"))
        std::cout << "Failed creature angry\n";

    if (!creatureScaredTexture.loadFromFile("Texture/creature_scared.png"))
        std::cout << "Failed creature scared\n";

    if (!creatureSideEyeTexture.loadFromFile("Texture/creature_sideeye.png"))
        std::cout << "Failed creature side eye\n";

    // stove backrounds

    if (!stoveEmptyTexture.loadFromFile("Texture/stove_layout.png"))
    {
        std::cout << "Failed to load stove pot texture\n";
    }
    /**/
    if (!stovePotTexture.loadFromFile("Texture/stove_with_pot.png"))
    { 
        std::cout << "Failed to load stove pot texture\n";
    }
    if (!stoveFlameTexture.loadFromFile("Texture/stove_with_flame.png"))
    {
        std::cout << "Failed to load stove flame texture\n";
    }

    //ingredients in pot

    if (!potIngredientsTexture.loadFromFile("Texture/spritesheet_ingredients_in_pot.png"))
    {
        std::cout << "Failed to load pot ingredients texture\n";
    }

    //plating

    if (!platingTexture.loadFromFile("Texture/plating_layout.png"))
    {
        std::cout << "Failed to load stove pot texture\n";
    }

    //TIMER 

    if (!timerFont.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    timerText.setCharacterSize(64);
    timerText.setFillColor(sf::Color::Black);

    timerText.setPosition({ 760.f, 150.f });

    //MUSIC

    initAudio();

     //sfx

    if (!knobOnBuffer.loadFromFile("SFX/stove_on.wav"))
        std::cout << "Failed to load cut sound\n";

    if (!knobOffBuffer.loadFromFile("SFX/stove_off.wav"))
        std::cout << "Failed to load peel sound\n";


    if (!gasOnBuffer.loadFromFile("SFX/gas.wav"))
        std::cout << "Failed to load gas sound\n";

    if (!waterOnBuffer.loadFromFile("SFX/water.wav"))
        std::cout << "Failed to load water sound\n";


    if (!selectBuffer.loadFromFile("SFX/select.wav"))
        std::cout << "Failed to load select sound\n";


    if (!enterBuffer.loadFromFile("SFX/enter.wav"))
        std::cout << "Failed to load enter sound\n";


    if (!popBuffer.loadFromFile("SFX/pop.wav"))
        std::cout << "Failed to load pop sound\n";


    if (!wrongBuzzBuffer.loadFromFile("SFX/wrong.wav"))
        std::cout << "Failed to load wrong buzz sound\n";


    if (!placeBuffer.loadFromFile("SFX/place.wav"))
        std::cout << "Failed to load place sound\n";


    if (!sizzleBuffer.loadFromFile("SFX/sizzle.wav"))
        std::cout << "Failed to load sizzle sound\n";

    if (!pourBuffer.loadFromFile("SFX/pour.wav"))
        std::cout << "Failed to load pour sound\n";

    if (!pourQuickBuffer.loadFromFile("SFX/pour_fast.wav"))
        std::cout << "Failed to load fast pour sound\n";


    if (!tryAgainBuffer.loadFromFile("SFX/try_again.wav"))
        std::cout << "Failed to try again sound\n";

    if (!goodBuffer.loadFromFile("SFX/good.wav"))
        std::cout << "Failed to good sound\n";

    if (!amazingBuffer.loadFromFile("SFX/amazing.wav"))
        std::cout << "Failed to load amazing sound\n";

    knobOnSound.emplace(knobOnBuffer);
    knobOffSound.emplace(knobOffBuffer);

    knobOnSound.value().setVolume(100.f);
    knobOffSound.value().setVolume(100.f);


    waterOnSound.emplace(waterOnBuffer);
    gasOnSound.emplace(gasOnBuffer);

    waterOnSound.value().setVolume(100.f);
    gasOnSound.value().setVolume(100.f);

    waterOnSound.value().setLooping(true);
    gasOnSound.value().setLooping(true);

    selectSound.emplace(selectBuffer);
    selectSound.value().setVolume(50.f);
    enterSound.emplace(enterBuffer);
    enterSound.value().setVolume(50.f);

    popSound.emplace(popBuffer);
    popSound.value().setVolume(50.f);

    wrongBuzzSound.emplace(wrongBuzzBuffer);
    wrongBuzzSound.value().setVolume(50.f);

    placeSound.emplace(placeBuffer);
    placeSound.value().setVolume(50.f);

    sizzleSound.emplace(sizzleBuffer);
    sizzleSound.value().setVolume(40.f);


    pourSound.emplace(pourBuffer);
    pourSound.value().setVolume(40.f);
    pourQuickSound.emplace(pourQuickBuffer);
    pourQuickSound.value().setVolume(40.f);

    tryAgainSound.emplace(tryAgainBuffer);
    tryAgainSound.value().setVolume(70.f);
    goodSound.emplace(goodBuffer);
    goodSound.value().setVolume(70.f);
    amazingSound.emplace(amazingBuffer);
    amazingSound.value().setVolume(70.f);


    // Add items ONCE

    // CARROT
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 0, 0 },          // whole
        { 1210, 560 },     // peeled
        { 1860, 560 },     // cut
        IngredientType::Carrot
    ));

    // PARSNIP
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 605, 0 },          // whole
        { 2465,560 },     // peeled
        { 0, 1120 },     // cut
        IngredientType::Parsnip
    ));

    // CHICKEN
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 1210,0 },          // whole
        {0,0}, //ignored
        { 605, 1120 },     // cut
        IngredientType::Chicken
    ));
    // CELERIAC
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 1860,0 },          // whole
        { 1210, 1120 },      //peeled
        { 1860, 1120 },     // cut
        IngredientType::Cereliac
    ));
    // PARSLEY
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 605,560 },          // whole
        { 0, 0},      //ignore
        { 0, 1680 },       // cut
        IngredientType::Parsley
    ));
    // GARLIC
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 2465,0},          // whole
        { 2465, 1120 },      //ignore
        { 2465, 1120 },       // cut
        IngredientType::Garlic
    ));
    // Buillion
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 0,560 },          // whole
        { 0,0 },      //ignore
        { 605, 1680 },       // cut
        IngredientType::Buillion
    ));

}

void StateManager::setState(std::unique_ptr<GameState> newState)
{

    if (dynamic_cast<MenuState*>(newState.get()))
    {
        resetGame();
        playMenuMusic();
    }
    else
    {
        playGameMusic();
    }

    currentState = std::move(newState);
}

void StateManager::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (currentState)
        currentState->handleEvent(window, event);
}

void StateManager::update()
{
    float current = gameTimer.getElapsedTime().asSeconds();
    float delta = current - lastFrameTime;
    lastFrameTime = current;

    // ONLY accumulate time when NOT paused
    if (!pauseTimer && showTimer)
    {
        elapsedTime += delta;
    }

    timeRemaining = gameDuration - elapsedTime;

        if (timeRemaining < 0.f)
        {
            timeRemaining = 0.f;
        }

        int minutes = static_cast<int>(timeRemaining) / 60;
        int seconds = static_cast<int>(timeRemaining) % 60;

        timerText.setString(
            std::to_string(minutes) + ":" +
            (seconds < 10 ? "0" : "") +
            std::to_string(seconds)
        );

        // NORMAL COLOR
        timerText.setFillColor(sf::Color::Black);

        // LAST 10 SECONDS then BLINK RED
        if (timeRemaining <= 10.f)
        {
            // blinking effect
            int blink = static_cast<int>(timeRemaining * 4) % 2;

            if (blink == 0)
            {
                timerText.setFillColor(sf::Color::Red);
            }
            else
            {
                timerText.setFillColor(sf::Color::White);
            }
        }

        if (timeRemaining <= 0.f && !gameOver)
        {
            gameOver = true;

            setState(std::make_unique<ResultState>(
                *this,
                ResultType::TimeUp
            ));
        }
    

    if (currentState)
        currentState->update();
}

void StateManager::draw(sf::RenderWindow& window)
{

    if (currentState)
        currentState->draw(window);


    /*
      //TIMER

    if (timerStarted)
    {
        window.draw(timerText);
    }
    */

    if (showTimer)
    {
        window.draw(timerText);
    }
  
}

std::unique_ptr<Item> StateManager::createIngredient(sf::Texture& texture, sf::Vector2i wholePos, sf::Vector2i peeledPos, sf::Vector2i cutPos, IngredientType type)
{
    return std::make_unique<Ingredient>(texture, sf::IntRect(wholePos, { 605, 560 }), sf::IntRect(peeledPos, { 605, 560 }), sf::IntRect(cutPos, { 605, 560 }), type);
}

std::unique_ptr<Pot> StateManager::createPot()
{

    return std::make_unique<Pot>(
        potTexture,

        //side view
        sf::IntRect({ 0,0 }, { 605,560 }),
        sf::IntRect({ 605,0 }, { 605,560 }),

        //top down view
        sf::IntRect({ 1210,0 }, { 605,560 }),
        sf::IntRect({ 1815,0 }, { 605,560 })
    );
}

std::unique_ptr<Jug> StateManager::createJug()
{
    return std::make_unique<Jug>(
        jugTexture,

        // empty jug
        sf::IntRect({ 1210,0 }, { 605,560}),

        // filled jug
        sf::IntRect({ 1815,0 }, { 605,560 })
    );
}

std::unique_ptr<Ladle> StateManager::createLadle()
{
    return std::make_unique<Ladle>(
        ladleTexture, 

        sf::IntRect({ 2420,0 }, { 605,560 }),   // empty
        sf::IntRect({ 0, 560 }, { 605, 560 })  // filled
    );
}

std::unique_ptr<Bowl> StateManager::createBowl()
{
    return std::make_unique<Bowl>(
        bowlTexture,

        sf::IntRect({ 605, 560 }, { 605, 560 }), //empty
        sf::IntRect({ 1210, 560 }, { 605, 560 }), //half full
        sf::IntRect({ 1815, 560 }, { 605, 560 })  //full
    );
}

void StateManager::addIngredientScore(CookState state)
{
    switch (state)
    {
    case CookState::Raw:
        soupScore += 0;
        break;

    case CookState::Cooked:
        soupScore += 15;
        break;

    case CookState::Overcooked:
        soupScore += 5;
        break;
    }

    std::cout << "Current soup score: " << soupScore << "\n";
}


void StateManager::setupCookingRects(CookingItem& cookingItem, IngredientType type)
{
    switch (type)
    {
    case IngredientType::Carrot:

        cookingItem.rawRect =
            sf::IntRect({ 0,0 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 605, 0 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 1210,0 }, { 605, 560 });

        break;

    case IngredientType::Cereliac:

        cookingItem.rawRect =
            sf::IntRect({ 1815,0 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 2420,0 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 0,560 }, { 605, 560 });

        break;

    case IngredientType::Parsnip:

        cookingItem.rawRect =
            sf::IntRect({ 605,560 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 1210,560 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 1815,560 }, { 605, 560 });

        break;
    
    case IngredientType::Chicken:

        cookingItem.rawRect =
            sf::IntRect({ 2420,560 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 0, 1120 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 605, 1120 }, { 605, 560 });

        break;

    case IngredientType::Parsley:

        cookingItem.rawRect =
            sf::IntRect({ 1210,1120 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 1815, 1120 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 2420, 1120 }, { 605, 560 });

        break;

    case IngredientType::Garlic:

        cookingItem.rawRect =
            sf::IntRect({ 0,1680 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 605, 1680 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 1210, 1680 }, { 605, 560 });

        break;
    case IngredientType::Buillion:

        cookingItem.rawRect =
            sf::IntRect({ 1815,1680 }, { 605, 560 });

        cookingItem.cookedRect =
            sf::IntRect({ 2420, 1680 }, { 605, 560 });

        cookingItem.overcookedRect =
            sf::IntRect({ 0, 2240 }, { 605, 560 });

        break;
    }
    
}

void StateManager::setupCookingTimes(CookingItem& cookingItem, IngredientType type)
{
    switch (type)
    {
    case IngredientType::Garlic:
        cookingItem.cookTime = 5.f;
        cookingItem.burnTime = 8.f;
        break;

    case IngredientType::Chicken:
        cookingItem.cookTime = 15.f;
        cookingItem.burnTime = 25.f;
        break;

    case IngredientType::Carrot:
        cookingItem.cookTime = 8.f;
        cookingItem.burnTime = 14.f;
        break;

    case IngredientType::Parsnip:
        cookingItem.cookTime = 7.f;
        cookingItem.burnTime = 12.f;
        break;

    case IngredientType::Parsley:
        cookingItem.cookTime = 3.f;
        cookingItem.burnTime = 6.f;
        break;

    case IngredientType::Cereliac:
        cookingItem.cookTime = 10.f;
        cookingItem.burnTime = 18.f;
        break;

    case IngredientType::Buillion:
        cookingItem.cookTime = 2.f;
        cookingItem.burnTime = 5.f;
        break;
    }
}


void StateManager::resetGame()
{
    soupScore = 0;
    soupFinished = false;

    stoveHasPot = false;
    stoveHeatOn = false;

    stovePotSpawned = false;

    potInInventory = false;
    jugInInventory = false;

    // clear old inventory
    inventory.clear();

    gameTimer.restart();
    timeRemaining = gameDuration;

    showTimer = false;
    timerStarted = false;

    gameOver = false;

    //reset stove dtata

    stovePot.reset();

    loadStartingInventory();

}

void StateManager::initAudio()
{
    if (!menuMusic.openFromFile("Music/puddleworld.ogg"))
        std::cout << "Failed menu music\n";

    menuMusic.setLooping(true);
    menuMusic.setVolume(50.f);

    if (!gameMusic.openFromFile("Music/comeaux.ogg"))
        std::cout << "Failed game music\n";

    gameMusic.setLooping(true);
    gameMusic.setVolume(30.f);
}

void StateManager::playMenuMusic()
{
    gameMusic.stop();

    if (menuMusic.getStatus() != sf::SoundSource::Status::Playing)
        menuMusic.play();
}

void StateManager::playGameMusic()
{
    menuMusic.stop();

    if (gameMusic.getStatus() != sf::SoundSource::Status::Playing)
        gameMusic.play();
}

void StateManager::stopGameMusic()
{
    gameMusic.stop();
}

void StateManager::loadStartingInventory()
{

    // CARROT
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 0, 0 },          // whole
        { 1210, 560 },     // peeled
        { 1860, 560 },     // cut
        IngredientType::Carrot
    ));

    // PARSNIP
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 605, 0 },          // whole
        { 2465,560 },     // peeled
        { 0, 1120 },     // cut
        IngredientType::Parsnip
    ));

    // CHICKEN
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 1210,0 },          // whole
        { 0,0 }, //ignored
        { 605, 1120 },     // cut
        IngredientType::Chicken
    ));
    // CELERIAC
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 1860,0 },          // whole
        { 1210, 1120 },      //peeled
        { 1860, 1120 },     // cut
        IngredientType::Cereliac
    ));
    // PARSLEY
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 605,560 },          // whole
        { 0, 0 },      //ignore
        { 0, 1680 },       // cut
        IngredientType::Parsley
    ));
    // GARLIC
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 2465,0 },          // whole
        { 2465, 1120 },      //ignore
        { 2465, 1120 },       // cut
        IngredientType::Garlic
    ));
    // Buillion
    inventory.addItem(createIngredient(
        ingredientsTexture,
        { 0,560 },          // whole
        { 0,0 },      //ignore
        { 605, 1680 },       // cut
        IngredientType::Buillion
    ));
}

    

