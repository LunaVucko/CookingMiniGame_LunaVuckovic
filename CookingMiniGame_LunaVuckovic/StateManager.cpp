#include "StateManager.h"
#include <iostream>


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

    // sink

    if (!sinkTexture.loadFromFile("Texture/sink_layout.png"))
    {
        std::cout << "Failed to load water texture\n";
    }


    if (!waterTexture.loadFromFile("Texture/sink_water.png"))
    {
        std::cout << "Failed to load water texture\n";
    }

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


    //testinhg it out with the carrot only, will add the other ingredients soon
    /*
    
    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 0,0 }, { 605,560 }), sf::IntRect({ 1210,560 }, { 605,560 }), sf::IntRect({ 1860,560 }, { 605,560 }), IngredientType::Carrot));

    //testinhg it out with the carrot only

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 605,0 }, { 605,560 }), IngredientType::Parsnip));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 1210,0 }, { 605,560 }), IngredientType::Chicken));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 1860,0 }, { 605,560 }), IngredientType::Cereliac));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 2465,0 }, { 605,560 }), IngredientType::Garlic));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 0,560 }, { 605,560 }), IngredientType::Buillion));

    inventory.addItem(std::make_unique<Ingredient>(
        ingredientsTexture, sf::IntRect({ 605,560 }, { 605,560 }), IngredientType::Parsley));

        */
}

void StateManager::setState(std::unique_ptr<GameState> newState)
{
    currentState = std::move(newState);
}

void StateManager::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (currentState)
        currentState->handleEvent(window, event);
}

void StateManager::update()
{
    if (currentState)
        currentState->update();
}

void StateManager::draw(sf::RenderWindow& window)
{
    if (currentState)
        currentState->draw(window);
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
    

