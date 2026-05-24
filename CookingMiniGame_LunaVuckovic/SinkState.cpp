#include "SinkState.h"
#include "PlayState.h"
#include <iostream>

SinkState::SinkState(StateManager& manager) : manager(manager)
{

    background.setSize({ 960, 720 });
   // background.setTexture(&texture);
    currentBackgroundTexture = &manager.sinkTexture;
    background.setTexture(currentBackgroundTexture);

    
    // sink area (adjust if needed)
    sinkArea = sf::FloatRect({ 300.f, 200.f }, { 350.f, 350.f });

    //water being turned on
    knobArea = sf::FloatRect({ 700.f, 290.f }, { 120.f, 120.f });

    //checks if pot is laready in inventory so it doesn't respawn when coming back to the sink

    /*
    *  if (!manager.potInInventory)
    {
        pot = manager.createPot();
        //manager.potInInventory = true;
        pot->sprite.setPosition({ 100.f, 500.f });
        pot->sprite.setScale({ 0.3f, 0.3f });
    }
    */
   

    if (!manager.jugInInventory)
    {
        jug = manager.createJug();

        //manager.jugInInventory = true;

        jug->sprite.setPosition({ 350.f, 500.f });

        jug->sprite.setScale({ 0.3f, 0.3f });
    }

   

}

void SinkState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    //inventory drag
    //manager.inventory.handleEvent(event);

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            // stop dragging ANY item
            if (auto* dragged = manager.inventory.getDraggedItem())
            {
                manager.wrongBuzzSound.value().play();
                dragged->isDragging = false;
            }
                
            /*
            * if (pot)
            {
                pot->isDragging = false;
            }
            */

            // also stop local pot if it still exists
            

            if (jug)
            {
                jug->isDragging = false;
            }
               


            manager.setState(std::make_unique<PlayState>(manager));

            //nextState = std::make_unique<PlayState>(manager);
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);


            /*
            * // pick up pot
            if (pot && pot->sprite.getGlobalBounds().contains(mousePos))
            {
                pot->isDragging = true;
                pot->dragOffset = pot->sprite.getPosition() - mousePos;
            }
            */
          

            //pick up the jug
            if (jug && jug->sprite.getGlobalBounds().contains(mousePos))
            {
                pendingDragItem = jug.get();

                pressStartPos = mousePos;

                mouseHeld = true;
            
            }

            if (knobArea.contains(mousePos))
            {
                isTurningKnob = true;
                knobTrail.clear();
            }
        }


    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        currentMousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };

        if (isTurningKnob)
        {
            knobTrail.push_back(currentMousePos);
        }

        manager.inventory.setMousePosition(currentMousePos);

        if (mouseHeld && pendingDragItem && !pendingDragItem->isDragging)
        {
            sf::Vector2f delta = currentMousePos - pressStartPos;

            if (std::hypot(delta.x, delta.y) > 10.f)
            {
                pendingDragItem->isDragging = true;

                pendingDragItem->dragOffset =
                    pendingDragItem->sprite.getPosition() - currentMousePos;

                mouseHeld = false;
                pendingDragItem = nullptr;
            }
        }


    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        /*
        if (pot && pot->isDragging && manager.inventory.contains(mousePos))
        {
            pot->isDragging = false;

            int slotIndex = manager.inventory.getSlotIndexAt(mousePos);

            if (slotIndex != -1)
            {

                manager.inventory.insertItemAt(std::move(pot), slotIndex);

            }
            else
            {
            
               manager.inventory.addItem(std::move(pot));

            }

            pot = nullptr; //the pot is gone after it has been moved so that ESC works (pointers are so weird)

            manager.potInInventory = true;

            std::cout << "Pot added to inventory!\n";

            return;
        }
        */

        

        if (jug && jug->isDragging && manager.inventory.contains(mousePos))
        {
            jug->isDragging = false;

            // prevent empty jug from entering inventory
            if (jug->state == JugState::Empty)
            {
                manager.wrongBuzzSound.value().play();
                jug->sprite.setPosition({ 350.f, 500.f });

                std::cout << "Fill the jug with water first!\n";

                return;
            }

            int slotIndex = manager.inventory.getSlotIndexAt(mousePos);

            if (slotIndex != -1)
            {
                manager.popSound.value().play();
                manager.inventory.insertItemAt(std::move(jug), slotIndex);
            }
            else
            {
                manager.popSound.value().play();
                manager.inventory.addItem(std::move(jug));
            }

            jug = nullptr;

            manager.jugInInventory = true;

            std::cout << "Jug added to inventory!\n";

            return;
        }


        if (jug && jug->isDragging && sinkArea.contains(mousePos))
        {
            jug->isDragging = false;

            if (waterOn)
            {
                if (jug->state == JugState::Empty)
                {
                    jug->state = JugState::Filled;

                    jug->updateSprite();

                    manager.pourSound.value().play();

                    std::cout << "Jug filled with water!\n";
                }
                else
                {
                    manager.wrongBuzzSound.value().play();
                    std::cout << "Jug already filled!\n";
                }
            }
            else
            {
                manager.wrongBuzzSound.value().play();
                std::cout << "Turn on water first!\n";
            }

            return;
        }

        /*
        
        if (pot && pot->isDragging && sinkArea.contains(mousePos))
        {
            pot->isDragging = false;

            std::cout << "You can't fill the pot directly! Use the jug.\n";

            return;
        }
        */

      

        if (isTurningKnob)
        {
            isTurningKnob = false;

            if (knobTrail.size() > 10)
            {
                float totalAngle = 0.f;

                for (size_t i = 2; i < knobTrail.size(); i++)
                {
                    sf::Vector2f a = knobTrail[i - 1] - knobTrail[i - 2];
                    sf::Vector2f b = knobTrail[i] - knobTrail[i - 1];

                    float cross = a.x * b.y - a.y * b.x;
                    float dot = a.x * b.x + a.y * b.y;

                    float angle = std::atan2(cross, dot);
                    totalAngle += angle;
                }

                // if roughly a circle (~360 degrees)
                if (std::abs(totalAngle) > 4.5f)
                {
                    waterOn = !waterOn;

                    currentBackgroundTexture = waterOn ? &manager.waterTexture : &manager.sinkTexture;

                    background.setTexture(currentBackgroundTexture);

                    if (waterOn)
                    {
                        manager.knobOnSound.value().play();
                        manager.waterOnSound.value().play();
                    }
                    else
                    {
                        manager.knobOffSound.value().play();
                        manager.waterOnSound.value().stop();
                    }
                         

                    std::cout << (waterOn ? "Water ON\n" : "Water OFF\n");
                }
            }

            knobTrail.clear();
        }

        mouseHeld = false;
        pendingDragItem = nullptr;
    }
}

void SinkState::update()
{
    /* if (pot && pot->isDragging)
    {
        pot->sprite.setPosition(currentMousePos + pot->dragOffset);
    }*/
   

    if (jug && jug->isDragging)
    {
        jug->sprite.setPosition(currentMousePos + jug->dragOffset);
    }
}

void SinkState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    /*
    
    //if (pot)
    if (pot)
    {
        window.draw(pot->sprite);
    }
    */


   

    // manager.inventory bar
    manager.inventory.draw(window);

    if (jug)
    {
        window.draw(jug->sprite);
    }

    /*
    *   // debug sink
    sf::RectangleShape debug;
    debug.setPosition(sinkArea.position);
    debug.setSize(sinkArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80)); // RED

    // debug sink knob

    sf::RectangleShape knobDebug;
    knobDebug.setPosition(knobArea.position);
    knobDebug.setSize(knobArea.size);
    knobDebug.setFillColor(sf::Color(0, 0, 255, 80)); // BLUE

    window.draw(knobDebug);



    window.draw(debug);
    */

  
}