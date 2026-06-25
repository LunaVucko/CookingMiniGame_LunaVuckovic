#include "StoveState.h"
#include "PlayState.h"
#include "ResultState.h"
#include <iostream>
#include "PlatingState.h"
#include <SFML/System/Angle.hpp>

StoveState::StoveState(StateManager& manager) : manager(manager)
{
 

    background.setSize({ 960, 720 });
   // background.setTexture(&manager.stoveEmptyTexture);


    // POT ALREADY PLACED ON STOVE
    if (manager.stoveHasPot)
    {
        background.setTexture(
            manager.stoveHeatOn
            ? &manager.stoveFlameTexture
            : &manager.stovePotTexture
        );

    }
    else {
        
            background.setTexture(&manager.stoveEmptyTexture);

            if (!manager.stovePot)
            {
                manager.stovePot = manager.createPot();

                manager.stovePot->sprite.setPosition({ 100.f, 500.f });
                manager.stovePot->sprite.setScale({ 0.3f, 0.3f });

               // manager.stovePotSpawned = true;
            }
    }
    /*
    else if (manager.stoveHeatOn)
    {
        background.setTexture(&manager.stoveFlameTexture);
    }
    else
    {
        background.setTexture(&manager.stovePotTexture);
    }
    */
        

    // Pot area and knob area
    potArea = sf::FloatRect({ 350.f, 200.f }, { 250.f, 250.f });
    knobArea = { {720.f,500.f},{120.f,120.f} };

    //knob indicator area

    knobIndicator.setTexture(&manager.rotateHintTexture);

    knobIndicator.setSize({ 250.f, 250.f });


    knobIndicator.setOrigin({
        knobIndicator.getSize().x / 2.f,
        knobIndicator.getSize().y / 2.f
        });



    knobIndicator.setPosition({ 780.f, 560.f });

    knobIndicator.setFillColor(sf::Color::Yellow);

   //drag indicator

    potArrow.setTexture(&manager.arrowJugHintTexture);

    potArrow.setSize({ 120.f, 120.f });

    potArrow.setOrigin({
        potArrow.getSize().x / 2.f,
        potArrow.getSize().y / 2.f
        });

    potArrow.setRotation(sf::degrees(-45.f));

    potArrowStart = { 180.f, 600.f };
    potArrowEnd = { 430.f, 350.f };

    potArrow.setFillColor(sf::Color::Yellow);

    showPotArrow = !manager.stoveHasPot;

}

void StoveState::spawnSmoke(sf::Vector2f position, sf::Color color)
{
    for (int i = 0; i < 30; i++)
    {
        SmokeParticle smoke;

        float size = 50.f + rand() % 10;

        smoke.shape.setRadius(size);
        smoke.shape.setFillColor(color);

        float offsetX = (rand() % 200 - 100);
        float offsetY = (rand() % 200 - 100);

        smoke.shape.setPosition({
            position.x + offsetX,
            position.y + offsetY
            });

        smoke.velocity = {
            (rand() % 20 - 10) * 0.1f,
            -(30.f + rand() % 30)
        };

        smoke.lifetime = 1.5f;

        smokeParticles.push_back(smoke);
    }
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

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);


             // pick up pot
            if (manager.stovePot && !manager.stoveHasPot && manager.stovePot->sprite.getGlobalBounds().contains(mousePos))
            {
                manager.stovePot->isDragging = true;
                manager.stovePot->dragOffset = manager.stovePot->sprite.getPosition() - mousePos;
            }
           
        }


    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        //mouse 
        currentMousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        manager.inventory.setMousePosition(currentMousePos);


    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();


        if (!mouse || mouse->button != sf::Mouse::Button::Left)
            return;

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (manager.stovePot && manager.stovePot->isDragging)
        {
            manager.stovePot->isDragging = false;

            if (potArea.contains(mousePos))
            {
                manager.stovePot = std::move(manager.stovePot);

                manager.stovePot->isOnStove = true;
                manager.stovePot->updateSprite();

                
                manager.stovePot->sprite.setScale({ 0.8f, 0.8f });
                manager.stovePot->sprite.setPosition({ 220.f, 100.f });

                manager.stoveHasPot = true;
                showPotArrow = false;
                showKnobIndicator = true;

                background.setTexture(&manager.stovePotTexture);

                manager.placeSound.value().play();

                std::cout << "Pot placed!\n";
            }

            return;
        }

        if (potArea.contains(mousePos))
        {

            auto item = manager.inventory.takeDraggedItem();
            if (!item)
            {
                return;
            }

            if (!manager.stoveHasPot)
            {
                manager.wrongBuzzSound.value().play();
                std::cout << "Can't add ingredient yet! Place pot first.\n";

                // put item back
                manager.inventory.addItem(std::move(item));

                return;
            }
            //if (!manager.stoveHasPot)
            //{
                /*
                // if it's pot then  place pot
                if (dynamic_cast<Pot*>(item.get()))
                {
                    manager.stovePot.reset(static_cast<Pot*>(item.release()));

                    manager.stovePot->isOnStove = true;
                    manager.stovePot->updateSprite();

                    manager.stovePot->sprite.setPosition({ 480.f, 350.f });
                    manager.stovePot->sprite.setScale({ 0.8f, 0.8f });

                    manager.stoveHasPot = true;
                    background.setTexture(&manager.stovePotTexture);
                    std::cout << "Pot placed\n";
                }
                else
                {
                    */
                //    std::cout << "Can't add ingredient yet! Place pot first.\n";

                    // return item back to inventory
                 //   manager.inventory.addItem(std::move(item));
                //}
                //return;
           // }
            if (auto* jug = dynamic_cast<Jug*>(item.get()))
            {

                if (jug->state != JugState::Filled)
                {
                    manager.wrongBuzzSound.value().play();
                    std::cout << "Jug is empty!\n";
                    manager.inventory.addItem(std::move(item));
                    return;
                }

                if (!manager.stovePot)
                {
                    manager.wrongBuzzSound.value().play();
                    std::cout << "Place pot first!\n";
                    manager.inventory.addItem(std::move(item));
                    return;
                }

                if (manager.stovePot->stoveItems.size() < manager.requiredIngredients)
                {
                    manager.wrongBuzzSound.value().play();

                    std::cout << "Add all the ingredients before adding the water!\n";

                    // return jug back to inventory
                    manager.inventory.addItem(std::move(item));

                    return;
                }

                if (potHasWater)
                {
                    manager.wrongBuzzSound.value().play();
                    std::cout << "Pot already has water!\n";
                    manager.inventory.addItem(std::move(item));
                    return;
                }

                potHasWater = true;

                // INIT WATER ONCE
                waterItem = CookingItem();
                // convert water into cooking item
                waterItem.item = std::make_unique<Item>(manager.potIngredientsTexture);
                waterItem.item->sprite.setTextureRect(waterRect);

                waterItem.cookState = CookState::Raw;
                waterItem.cookingClock.restart();
                waterItem.isCookingStarted = true;

                waterInitialized = true;


                std::cout << "Water added to pot!\n";

                return;
            }
                        // check if it's an ingredient
                        if (auto* ingredient = dynamic_cast<Ingredient*>(item.get()))
                        {
                            // allow ONLY cut ingredients
                            if (ingredient->state == IngredientState::Cut)
                            {
                                // make the sprites bigger
                                ingredient->sprite.setScale({ 0.4f, 0.4f });

                                // position inside pot (optional but nice)
                                ingredient->sprite.setPosition({
                                    potArea.position.x + 80.f + (rand() % 50),
                                    potArea.position.y + 100.f + (rand() % 50)
                                    });

                                //manager.stoveItems.push_back(std::move(item));
                                Ingredient* ingPtr = dynamic_cast<Ingredient*>(item.get());
                                if (!ingPtr)
                                {
                                    return;
                                }

                                IngredientType ingredientType = ingPtr->type;
                                //IngredientState ingredientState = ingPtr->state;

                                CookingItem cookingItem;
                                cookingItem.item = std::move(item);

                                cookingItem.cookState = CookState::Raw;
                                cookingItem.cookingClock.restart();
                                cookingItem.isCookingStarted = true;

                                manager.setupCookingRects(cookingItem, ingredientType);
                                manager.setupCookingTimes(cookingItem, ingredientType);

                                cookingItem.item->sprite.setTexture(manager.potIngredientsTexture);
                                cookingItem.item->sprite.setTextureRect(cookingItem.rawRect);

                                manager.stovePot->stoveItems.push_back(std::move(cookingItem));

                                manager.stovePot->state = PotState::Filled;
                                manager.stovePot->updateSprite();

                                manager.stovePot->activeCookingItem = &manager.stovePot->stoveItems.back();

                                manager.sizzleSound.value().play();

                                std::cout << "Cut ingredient added\n";
                            }
                            else
                            {
                                manager.wrongBuzzSound.value().play();
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

            if (manager.stoveHeatOn)
            {
                showKnobIndicator = false;
                manager.knobOnSound.value().play();
                manager.gasOnSound.value().play();
            }
            else
            {
                manager.knobOffSound.value().play();
                manager.gasOnSound.value().stop();
            }
          

            std::cout << (manager.stoveHeatOn ? "Heat ON\n" : "Heat OFF\n");
        }
    }
}

void StoveState::update()
{

    if (showKnobIndicator)
    {
        indicatorRotation += -30.f * 0.016f;

        knobIndicator.setRotation(sf::degrees(indicatorRotation));
    }

    if (showPotArrow)
    {
        potArrowAnimTime += 0.008f;

        float t =
            (std::sin(potArrowAnimTime * 0.8f) + 1.f) * 0.5f;

        sf::Vector2f pos =
        {
            potArrowStart.x +
            (potArrowEnd.x - potArrowStart.x) * t,

            potArrowStart.y +
            (potArrowEnd.y - potArrowStart.y) * t
        };

        potArrow.setPosition(pos);
    }


    if (manager.stovePot && manager.stovePot->isDragging)
    {
        manager.stovePot->sprite.setPosition(currentMousePos + manager.stovePot->dragOffset);
    }

    manager.inventory.update();

    //if (manager.stoveHeatOn)
    //{
        //for (auto& cookingItem : manager.stoveItems)
        //{
    if (!manager.stoveHeatOn)
    {
        return;
    }
    if (!manager.stovePot)
    {
        return;
    }
    // No active item = nothing cooks
    if (!manager.stovePot->activeCookingItem)
    {
        return;
    }

    bool cookingPaused = potHasWater && waterInitialized;

    CookingItem& cookingItem = *manager.stovePot->activeCookingItem;

    float time = cookingItem.cookingClock.getElapsedTime().asSeconds();


    if (!cookingPaused)
    {
        // RAW becomes COOKED
        if (time >= cookingItem.cookTime && cookingItem.cookState == CookState::Raw)
        {
            cookingItem.cookState = CookState::Cooked;
            cookingItem.item->sprite.setTextureRect(cookingItem.cookedRect);

            sf::FloatRect potBounds = manager.stovePot->sprite.getGlobalBounds();

            sf::Vector2f smokePos(
                potBounds.position.x + potBounds.size.x / 3.f,
                potBounds.position.y + potBounds.size.y / 4.f
            );

            spawnSmoke(
                smokePos,
                sf::Color(220, 220, 220, 180)
            );

            std::cout << "Ingredient cooked\n";
        }

        // COOKED becomes OVERCOOKED
        else if (time >= cookingItem.burnTime && cookingItem.cookState == CookState::Cooked)
        {
            cookingItem.cookState = CookState::Overcooked;
            cookingItem.item->sprite.setTextureRect(cookingItem.overcookedRect);

            sf::FloatRect potBounds = manager.stovePot->sprite.getGlobalBounds();

            sf::Vector2f smokePos(
                potBounds.position.x + potBounds.size.x / 3.f,
                potBounds.position.y + potBounds.size.y / 4.f
            );

            spawnSmoke(
                smokePos,
                sf::Color(220, 220, 220, 180)
            );

            std::cout << "Ingredient burned\n";
        }
    }

    if (potHasWater && waterInitialized && manager.stoveHeatOn && !manager.soupFinished)
    {
        float t = waterItem.cookingClock.getElapsedTime().asSeconds();

        if (t >= 5.f && waterItem.cookState == CookState::Raw)
        {
            waterItem.cookState = CookState::Cooked;

            waterItem.item->sprite.setTextureRect(waterCookedRect);

            std::cout << "Water is boiling!\n";

            // calculate final soup score
            manager.soupScore = 0;

            for (auto& cookingItem : manager.stovePot->stoveItems)
            {
                manager.addIngredientScore(cookingItem.cookState);
            }

            manager.soupFinished = true;

           

            std::cout << "Soup complete!\n";
            std::cout << "Final score: " << manager.soupScore << "\n";

            //manager.setState(std::make_unique<ResultState>(manager));

            /*  manager.setState(std::make_unique<ResultState>(
                manager,
                ResultType::Score
            ));*/
          
            manager.gasOnSound.value().stop();

            manager.setState(std::make_unique<PlatingState>(manager));

            return;
        }

        
    }

    for (size_t i = 0; i < smokeParticles.size(); )
    {
        smokeParticles[i].lifetime -= 0.016f;

        smokeParticles[i].shape.move(
            smokeParticles[i].velocity * 0.016f
        );

        sf::Color c = smokeParticles[i].shape.getFillColor();

        if (c.a > 3)
            c.a -= 3;

        smokeParticles[i].shape.setFillColor(c);

        if (smokeParticles[i].lifetime <= 0.f)
        {
            smokeParticles.erase(smokeParticles.begin() + i);
        }
        else
        {
            i++;
        }
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
    
    if (manager.stovePot)
    {
        window.draw(manager.stovePot->sprite);


        // Pot ingredients
        for (auto& cookingItem : manager.stovePot->stoveItems)
        {
            window.draw(cookingItem.item->sprite);
        }

        // draw water layer

        if (potHasWater && waterInitialized)
        {
            sf::Vector2f potPos = manager.stovePot->sprite.getPosition();

            waterItem.item->sprite.setPosition({
                potPos.x - 0.f,
                potPos.y - 0.f
                });


            waterItem.item->sprite.setScale(manager.stovePot->sprite.getScale());


            window.draw(waterItem.item->sprite);
        }
    }

    if (showPotArrow)
    {
        window.draw(potArrow);
    }

    if (showKnobIndicator)
    {
        window.draw(knobIndicator);
    }

    for (auto& smoke : smokeParticles)
    {
        window.draw(smoke.shape);
    }

  /*  // debug
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

    window.draw(knobDebug);*/
}