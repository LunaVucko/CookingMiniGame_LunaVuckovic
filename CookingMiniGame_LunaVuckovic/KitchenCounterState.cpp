#include "KitchenCounterState.h"
#include "PlayState.h"
#include <iostream>

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
    //inventory drag
    manager.inventory.handleEvent(event);

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

            //nextState = std::make_unique<PlayState>(manager);
        }
    }


    // mouse press
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                (float)mouse->position.x,
                (float)mouse->position.y
            );

            
            for (auto& tool : toolItems) // <= clicking on the tool
            {
                if (tool.sprite.getGlobalBounds().contains(mousePos))
                {
                    currentTool = tool.type;
                    std::cout << "Tool selected!\n";
                    return; 
                }
            }

            for (auto& ing : counterIngredients)
            {
                if (ing->sprite.getGlobalBounds().contains(mousePos))
                {
                    if (currentTool == ToolType::Peeler && ing->state == IngredientState::Whole)
                    {
                        ing->state = IngredientState::Peeled;
                        ing->updateSprite();
                        std::cout << "Peeled!\n";
                    }

                    else if (currentTool == ToolType::Knife && ing->state == IngredientState::Peeled)
                    {
                        ing->state = IngredientState::Cut;
                        ing->updateSprite();
                        std::cout << "Cut!\n";
                    }
                }
            }
        }


    }

    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (cuttingBoardArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (item)
            {
                // place item at board center
                item->sprite.setPosition({ 475.f, 375.f });

                counterIngredients.push_back(move(item));
            }
        }

        if (manager.inventory.contains(mousePos))
        {
            auto item = takeCounterItem(mousePos);
            if (item)
        {
            if (manager.inventory.contains(mousePos))
            {
                manager.inventory.addItem(std::move(item));
            }
            else
            {
                // drop back if not over inventory
                counterIngredients.push_back(std::move(item));
            }
        }
        }

    }
}

std::unique_ptr<Ingredient> KitchenCounterState::takeCounterItem(sf::Vector2f mousePos)
{
    for (size_t i = 0; i < counterIngredients.size(); i++)
    {
        if (counterIngredients[i]->sprite.getGlobalBounds().contains(mousePos))
        {
            auto ptr = std::move(counterIngredients[i]);
            counterIngredients.erase(counterIngredients.begin() + i);
            return ptr;
        }
    }
    return nullptr;
}

   

void KitchenCounterState::update()
{
    // Nothing yet
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

