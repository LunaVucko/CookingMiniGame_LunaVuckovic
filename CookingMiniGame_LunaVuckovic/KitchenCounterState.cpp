#include "KitchenCounterState.h"
#include "PlayState.h"
#include <iostream>
#include <cmath>

KitchenCounterState::KitchenCounterState(StateManager& manager) : manager(manager)
{
    if (!texture.loadFromFile("Texture/counter_layout.png")) // <= background
    {
        std::cout << "Failed to load counter texture\n";
    }

    background.setSize({ 960, 720 });
    background.setTexture(&texture);
    
    // Cutting board area (adjust if needed)
     cuttingBoardArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

     //spritesheet

     if (!toolsTexture.loadFromFile("Texture/tools_spritesheet.png"))
     {
         std::cout << "Failed to load tools spritesheet\n";
     }

     // adding the tools

     toolItems.emplace_back(toolsTexture, sf::IntRect({ 0,0 }, { 605,560 }), ToolType::Knife);
     toolItems.emplace_back(toolsTexture, sf::IntRect({ 605,0 }, { 605,560 }), ToolType::Peeler);

     // position for the tools

     for (int i = 0; i < toolItems.size(); i++)
     {
         toolItems[i].sprite.setPosition({ 10.f + i * 90.f, 630.f });
         toolItems[i].sprite.setScale({ 0.1f, 0.1f });
     }

}

void KitchenCounterState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            //stops the item apearing on the hand when esc
            if (auto* dragged = manager.inventory.getDraggedItem())
                dragged->isDragging = false;

            manager.setState(std::make_unique<PlayState>(manager));

        }
    }




    // mouse press
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        //inventory drag
        manager.inventory.handleEvent(event);

        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                (float)mouse->position.x,
                (float)mouse->position.y
            );

            dragStart = mousePos;
            isDraggingGesture = true;
            
            for (auto& tool : toolItems) // <= clicking on the tool
            {
                if (tool.sprite.getGlobalBounds().contains(mousePos))
                {
                    currentTool = tool.type;
                    std::cout << "Tool selected!\n";
                    return; 
                }
            }

            // selecting the ingredient
            for (auto& item : counterIngredients)
            {
                if (item->sprite.getGlobalBounds().contains(mousePos))
                {
                    selectedIngredient = item.get();

                    // ONLY DRAG if NO tool selected
                    if (currentTool == ToolType::None)
                    {
                        item->isDragging = true;
                        item->dragOffset = item->sprite.getPosition() - mousePos;
                    }
                    return;
                }
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
     
        manager.inventory.setMousePosition(currentMousePos);
    }



    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        dragEnd = mousePos;

        // Mouse action movement logic
        if (isDraggingGesture && selectedIngredient)
        {
            sf::Vector2f delta = dragEnd - dragStart;

            float dx = std::abs(delta.x);
            float dy = std::abs(delta.y);

            if (std::hypot(dx, dy) > 30.f) // ignore tiny movement
            {
                //CUT (diagonal)
                if (currentTool == ToolType::Knife && dx > 20.f && dy > 20.f)
                {
                    if (canCut(selectedIngredient->type))
                    {
                        bool canCutNow = false;

                        if (selectedIngredient->type == IngredientType::Carrot ||
                            selectedIngredient->type == IngredientType::Parsnip ||
                            selectedIngredient->type == IngredientType::Cereliac)
                        {
                            canCutNow = (selectedIngredient->state == IngredientState::Peeled);
                        }
                        else
                        {
                            canCutNow = (selectedIngredient->state == IngredientState::Whole);
                        }

                        if (canCutNow)
                        {
                            selectedIngredient->state = IngredientState::Cut;
                            selectedIngredient->updateSprite();
                            currentTool = ToolType::None; //unselect the tool
                            std::cout << "Cut!\n";
                        }
                        else
                        {
                            std::cout << "Can't cut yet!\n";
                        }
                    }
                }

                //PEEL (horizontal)
                else if (currentTool == ToolType::Peeler && dx > dy)
                {
                    if (selectedIngredient->state == IngredientState::Whole &&
                        canPeel(selectedIngredient->type))
                    {
                        selectedIngredient->state = IngredientState::Peeled;
                        selectedIngredient->updateSprite();
                        currentTool = ToolType::None;//unselect the tool
                        std::cout << "Peeled!\n";
                    }
                    else
                    {
                        std::cout << "Can't peel this!\n";
                    }
                }
            }
        }

        isDraggingGesture = false;
        selectedIngredient = nullptr;

        //from inventoy to counter

        if (cuttingBoardArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (item)
            {
                item->isDragging = false;
                // place item at board center
                item->sprite.setPosition({ 475.f, 375.f });

                counterIngredients.push_back(move(item));
                return;
            }
        }

        //counter to inventory

        for (size_t i = 0; i < counterIngredients.size(); i++)
        {
            if (counterIngredients[i]->isDragging)
            {
                if (manager.inventory.contains(mousePos))
                {
                    auto item = std::move(counterIngredients[i]);
                    counterIngredients.erase(counterIngredients.begin() + i);

                    item->isDragging = false;
                    //manager.inventory.addItem(std::move(item));
                    int slotIndex = manager.inventory.getSlotIndexAt(mousePos);

                    if (slotIndex != -1)
                    {
                        item->isDragging = false;
                        manager.inventory.insertItemAt(std::move(item), slotIndex);
                    }
                }
                else
                {
                    // snap back if not dropped in inventory
                    counterIngredients[i]->isDragging = false;
                }
                return;
            }
        }

        // Reset!
        selectedIngredient = nullptr;

    }
}  

void KitchenCounterState::update()
{
    manager.inventory.update();

   // sf::Vector2f mousePos = manager.inventory.getMousePos(); 

    for (auto& ing : counterIngredients)
    {
        if (ing->isDragging)
        {
            ing->sprite.setPosition(currentMousePos + ing->dragOffset);
        }
    }

 
}

void KitchenCounterState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    // Inventory bar <= top inventory
    manager.inventory.draw(window);

    // Cutting board ingredients
    for (auto& ing : counterIngredients)
    {
        window.draw(ing->sprite);
    }

    // tools

    for (auto& tool : toolItems)
    {
        window.draw(tool.sprite);
    }

    // debug
    sf::RectangleShape debug;
    debug.setPosition( cuttingBoardArea.position);
    debug.setSize( cuttingBoardArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);
}

