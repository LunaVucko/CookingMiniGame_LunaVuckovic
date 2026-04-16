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

    // Inventory bar
    inventoryBar.setSize({ 960, 100 });
    inventoryBar.setPosition({ 0, 0 });
    inventoryBar.setFillColor(sf::Color::White);
    inventoryBar.setOutlineColor(sf::Color::Black);
    inventoryBar.setOutlineThickness(2.f);

    //Inventory slots

    float slotSize = 80.f;
    float spacing = 10.f;
    float startX = 10.f;

    for (int i = 0; i < 10; i++)
    {
        sf::RectangleShape slot;
        slot.setSize({ slotSize, slotSize });
        slot.setPosition({ startX + i * (slotSize + spacing), 10.f });

        slot.setFillColor(sf::Color::Transparent);
        slot.setOutlineColor(sf::Color::Black);
        slot.setOutlineThickness(2.f);

        slots.push_back(slot);
    }

    //spritesheet

    if (!ingredientsTexture.loadFromFile("Texture/spritesheet_V2.png")) // <= ingreients assets
    {
        std::cout << "Failed to load spritesheet\n";
    }

    // Add ingredients

    auto addIngredient = [&](const sf::IntRect& rect)
        {
            auto ing = std::make_unique<Ingredient>(ingredientsTexture, rect);
            inventory.push_back(std::move(ing));

        };

    addIngredient(sf::IntRect({ 0, 0 }, { 605, 560 })); //carrot
    addIngredient(sf::IntRect({ 605, 0 }, { 605, 560 })); //parsnip
    addIngredient(sf::IntRect({ 1210, 0 }, { 605, 560 })); //chicken breast
    addIngredient(sf::IntRect({ 1860, 0 }, { 605, 560 })); //cerialac
    addIngredient(sf::IntRect({ 2465, 0 }, { 605, 560 })); //garlic
    addIngredient(sf::IntRect({ 0, 560 }, { 605, 560 })); // soup
    addIngredient(sf::IntRect({ 605, 560 }, { 605, 560 })); //parsley

    // Position ingredients + scale the ingredients within the slots of the inventory
    for (size_t i = 0; i < inventory.size(); i++)
    {
        auto& ing = inventory[i];

        sf::FloatRect bounds = ing->sprite.getLocalBounds();

        float scaleX = slotSize / bounds.size.x;
        float scaleY = slotSize / bounds.size.y;
        float scale = std::min(scaleX, scaleY);

        ing->sprite.setScale({ scale, scale });

        // center origin
        ing->sprite.setOrigin({
            bounds.size.x / 2.f,
            bounds.size.y / 2.f
            });

        // center in slot
        ing->sprite.setPosition({
            slots[i].getPosition().x + slotSize / 2.f,
            slots[i].getPosition().y + slotSize / 2.f
            });
    }

    // Cutting board area (adjust if needed)
     cuttingBoardArea = sf::FloatRect({ 400.f, 300.f }, { 150.f, 150.f });

}

void KitchenCounterState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));

            //nextState = std::make_unique<PlayState>(manager);
        }
    }

    // pressing the mouse starts the dragging
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                (float)mouse->position.x,
                (float)mouse->position.y
            );

            for (auto& ing : inventory)
            {
                if (ing->sprite.getGlobalBounds().contains(mousePos) &&
                    inventoryBar.getGlobalBounds().contains(mousePos))
                {
                    ing->isDragging = true;
                }
            }
        }
    }

    // when mooving the mouse the ingredients are being moved ( dragged )
    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        sf::Vector2f mousePos(
            (float)mouse->position.x,
            (float)mouse->position.y
        );

        for (auto& ing : inventory)
        {
            if (ing->isDragging)
            {
                ing->sprite.setPosition(mousePos);
            }
        }
    }

    // realasing the mouse drops the items
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos(
            (float)mouse->position.x,
            (float)mouse->position.y
        );

        for (size_t i = 0; i < inventory.size(); i++)
        {
            auto& ing = inventory[i];

            if (ing->isDragging)
            {
                if ( cuttingBoardArea.contains(mousePos))
                {
                    // Move safely to cutting board
                    counterIngredients.push_back(std::move(ing));

                    // Remove from inventory
                    inventory.erase(inventory.begin() + i);
                    break;
                }

                ing->isDragging = false;
            }
        }
    }
}

void KitchenCounterState::update()
{
    // Nothing yet
}

void KitchenCounterState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    // Inventory bar
    window.draw(inventoryBar);

    //slots in the inventory bar
    for (auto& slot : slots)
    {
        window.draw(slot);
    }

    // Inventory items
    for (auto& ing : inventory)
    {
        window.draw(ing->sprite);
    }

    // Cutting board ingredients
    for (auto& ing : counterIngredients)
    {
        window.draw(ing->sprite);
    }

    // debug
    sf::RectangleShape debug;
    debug.setPosition( cuttingBoardArea.position);
    debug.setSize( cuttingBoardArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);
}