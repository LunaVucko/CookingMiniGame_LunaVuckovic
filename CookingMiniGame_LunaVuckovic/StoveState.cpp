#include "StoveState.h"
#include "PlayState.h"
#include <iostream>

StoveState::StoveState(StateManager& manager) : manager(manager)
{
 

    background.setSize({ 960, 720 });
   // background.setTexture(&manager.stoveEmptyTexture);


    if (!manager.stoveHasPot)
    {
        background.setTexture(&manager.stoveEmptyTexture);
    }
    else if (manager.stoveHeatOn)
    {
        background.setTexture(&manager.stoveFlameTexture);
    }
    else
    {
        background.setTexture(&manager.stovePotTexture);
    }
        


    // Pot area and knob area
    potArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });
    knobArea = { {720.f,500.f},{120.f,120.f} };

}

void StoveState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    //inventory drag
    manager.inventory.handleEvent(event);

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

           // nextState = std::make_unique<PlayState>(manager);
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        sf::Vector2f mousePos(
            (float)mouse->position.x,
            (float)mouse->position.y
        );

        manager.inventory.setMousePosition(mousePos);
    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (potArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (!item)
            {
                return;
            }
            if (!manager.stoveHasPot)
            {
                // if it's pot then  place pot
                if (dynamic_cast<Pot*>(item.get()))
                {
                    stovePot.reset(static_cast<Pot*>(item.release()));

                    manager.stoveHasPot = true;
                    background.setTexture(&manager.stovePotTexture);
                    std::cout << "Pot placed\n";
                }
                else
                {
                    std::cout << "Can't add ingredient yet! Place pot first.\n";

                    // return item back to inventory
                    manager.inventory.addItem(std::move(item));
                }
                return;
            }
        
                        // check if it's an ingredient
                        if (auto* ingredient = dynamic_cast<Ingredient*>(item.get()))
                        {
                            // allow ONLY cut ingredients
                            if (ingredient->state == IngredientState::Cut)
                            {
                                // make the sprites bigger
                                ingredient->sprite.setScale({ 0.6f, 0.6f });

                                // position inside pot (optional but nice)
                                ingredient->sprite.setPosition({
                                    potArea.position.x + 40.f + (rand() % 50),
                                    potArea.position.y + 40.f + (rand() % 50)
                                    });

                                //manager.stoveItems.push_back(std::move(item));
                                Ingredient* ingPtr = dynamic_cast<Ingredient*>(item.get());
                                if (!ingPtr)
                                {
                                    return;
                                }

                                IngredientType ingredientType = ingPtr->type;
                                IngredientState ingredientState = ingPtr->state;

                                CookingItem cookingItem;
                                cookingItem.item = std::move(item);

                                cookingItem.cookState = CookState::Raw;
                                cookingItem.cookingClock.restart();
                                cookingItem.isCookingStarted = true;

                                manager.setupCookingRects(cookingItem, ingredientType);

                                cookingItem.item->sprite.setTexture(manager.potIngredientsTexture);
                                cookingItem.item->sprite.setTextureRect(cookingItem.rawRect);

                                stovePot->stoveItems.push_back(std::move(cookingItem));

                                stovePot->activeCookingItem = &stovePot->stoveItems.back();

                                std::cout << "Cut ingredient added\n";
                            }
                            else
                            {
                                std::cout << "Ingredient must be CUT first!\n";

                                // put it back into inventory
                                manager.inventory.addItem(std::move(item));
                            }
                        }
                        else
                        {
                            // not an ingredient (just in case)
                            manager.inventory.addItem(std::move(item));
                        }
            
               
                
        }

        // KNOB CLICK
        if (knobArea.contains(mousePos) && manager.stoveHasPot)
        {
            manager.stoveHeatOn = !manager.stoveHeatOn;

            background.setTexture(manager.stoveHeatOn ? &manager.stoveFlameTexture: &manager.stovePotTexture);

            std::cout << (manager.stoveHeatOn ? "Heat ON\n" : "Heat OFF\n");
        }
    }
}

void StoveState::update()
{
    manager.inventory.update();

    //if (manager.stoveHeatOn)
    //{
        //for (auto& cookingItem : manager.stoveItems)
        //{
            if (!manager.stoveHeatOn)
            {
               return;
            }
            if (!stovePot)
            {
                return;
            }
            // No active item = nothing cooks
            if (!stovePot->activeCookingItem)
            {
                return;
            }

            CookingItem& cookingItem = *stovePot->activeCookingItem;

            float time = cookingItem.cookingClock.getElapsedTime().asSeconds();

            // RAW becomes COOKED
            if (time >= 5.f && cookingItem.cookState == CookState::Raw)
            {
                cookingItem.cookState = CookState::Cooked;
                cookingItem.item->sprite.setTextureRect(cookingItem.cookedRect);
                std::cout << "Ingredient cooked\n";
            }

            // COOKED becomes OVERCOOKED
            else if (time >= 10.f && cookingItem.cookState == CookState::Cooked)
            {
                cookingItem.cookState = CookState::Overcooked;
                cookingItem.item->sprite.setTextureRect(cookingItem.overcookedRect);
                std::cout << "Ingredient burned\n";
            }
      //  }
    //}
}

void StoveState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    // Inventory bar
    manager.inventory.draw(window);

    if (auto* dragged = manager.inventory.getDraggedItem())
    {
        window.draw(dragged->sprite);
    }
    if (stovePot)
    {
        // Pot ingredients
        for (auto& cookingItem : stovePot->stoveItems)
        {
            window.draw(cookingItem.item->sprite);
        }
    }

    // debug
    sf::RectangleShape debug;
    debug.setPosition(potArea.position);
    debug.setSize(potArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);

    // debug sink knob

    sf::RectangleShape knobDebug;
    knobDebug.setPosition(knobArea.position);
    knobDebug.setSize(knobArea.size);
    knobDebug.setFillColor(sf::Color(0, 0, 255, 80)); // BLUE

    window.draw(knobDebug);
}