#include "Inventory.h"

Inventory::Inventory()
{
    inventoryBar.setSize({ 960, 100 });
    inventoryBar.setPosition({ 0, 0 });
    inventoryBar.setFillColor(sf::Color::White);
    inventoryBar.setOutlineColor(sf::Color::Black);
    inventoryBar.setOutlineThickness(2.f);

    setupSlots();
}

void Inventory::setupSlots()
{
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
}

void Inventory::addItem(std::unique_ptr<Ingredient> item)
{
    size_t i = items.size();

    // scale + center
    sf::FloatRect bounds = item->sprite.getLocalBounds();

    float scaleX = slotSize / bounds.size.x;
    float scaleY = slotSize / bounds.size.y;
    float scale = std::min(scaleX, scaleY);

    item->sprite.setScale({ scale, scale });

    item->sprite.setOrigin({
        bounds.size.x / 2.f,
        bounds.size.y / 2.f
        });

    item->sprite.setPosition({
        slots[i].getPosition().x + slotSize / 2.f,
        slots[i].getPosition().y + slotSize / 2.f
        });

    items.push_back(std::move(item));
}

bool Inventory::contains(sf::Vector2f point) const
{
    return inventoryBar.getGlobalBounds().contains(point);
}

void Inventory::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

            for (auto& ing : items)
            {
                if (ing->sprite.getGlobalBounds().contains(mousePos))
                {
                    ing->isDragging = true;
                }
            }
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        for (auto& ing : items)
        {
            if (ing->isDragging)
            {
                ing->sprite.setPosition(mousePos);
            }
        }
    }
}

Ingredient* Inventory::getDraggedItem()
{
    for (auto& ing : items)
    {
        if (ing->isDragging)
            return ing.get();
    }
    return nullptr;
}

std::unique_ptr<Ingredient> Inventory::takeDraggedItem()
{
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i]->isDragging)
        {
            auto ptr = std::move(items[i]);
            items.erase(items.begin() + i);
            return ptr;
        }
    }
    return nullptr;
}

void Inventory::draw(sf::RenderWindow& window)
{
    window.draw(inventoryBar);

    for (auto& slot : slots)
        window.draw(slot);

    for (auto& ing : items)
        window.draw(ing->sprite);
}