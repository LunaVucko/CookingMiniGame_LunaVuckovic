#include "Inventory.h"

Inventory::Inventory(InventoryType type)
{
    if (type == InventoryType::Top)
    {
        yOffset = 0.f;
    }
    else
    {
        yOffset = 620.f; // bottom (720 - 100)
    }
        

    inventoryBar.setSize({ 960, 100 });
    inventoryBar.setPosition({ 0, yOffset });
    inventoryBar.setFillColor(sf::Color::White);
    inventoryBar.setOutlineColor(sf::Color::Black);
    inventoryBar.setOutlineThickness(2.f);

    setupSlots();

    /*inventoryBar.setSize({960, 100});
    inventoryBar.setPosition({ 0, 0 });
    inventoryBar.setFillColor(sf::Color::White);
    inventoryBar.setOutlineColor(sf::Color::Black);
    inventoryBar.setOutlineThickness(2.f);

    setupSlots();
    */
}

void Inventory::setupSlots()
{
    float spacing = 10.f;
    float startX = 10.f;

    for (int i = 0; i < 10; i++)
    {
        sf::RectangleShape slot;
        slot.setSize({ slotSize, slotSize });
        //slot.setPosition({ startX + i * (slotSize + spacing), 10.f });
        slot.setPosition({ startX + i * (slotSize + spacing), yOffset + 10.f });

        slot.setFillColor(sf::Color::Transparent);
        slot.setOutlineColor(sf::Color::Black);
        slot.setOutlineThickness(2.f);

        slots.push_back(slot);
    }
}

void Inventory::addItem(std::unique_ptr<Ingredient> item)
{
    if (items.size() >= slots.size())
    {
        cout << "too many items!!";
        return;
    }

    size_t index = items.size();

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

    item->isDragging = false;

    items.push_back(std::move(item));

   /* item->sprite.setPosition({
        slots[index].getPosition().x + slotSize / 2.f,
        slots[index].getPosition().y + slotSize / 2.f
        });

    items.push_back(std::move(item));
    */
    //rea-align all items after pushing

    for (size_t i = 0; i < items.size(); i++)
    {
        if (!items[i]->isDragging)
        {

            items[i]->sprite.setPosition({
                slots[i].getPosition().x + slotSize / 2.f,
                slots[i].getPosition().y + slotSize / 2.f
                });
        }
    }
}

bool Inventory::contains(sf::Vector2f point) const
{
    return inventoryBar.getGlobalBounds().contains(point);
}

bool Inventory::isEmpty() const
{
    return items.empty();
}

void Inventory::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

            // IMPORTANT: only one drag allowed
            for (auto& ing : items)
                ing->isDragging = false;

            for (auto& ing : items)
            {
                if (ing->sprite.getGlobalBounds().contains(mousePos))
                {
                    ing->isDragging = true;
                    ing->dragOffset = ing->sprite.getPosition() - mousePos;
                }
            }
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);
/*for (auto& ing : items)
        {
            if (ing->isDragging)
            {
                ing->sprite.setPosition(mousePos + ing->dragOffset);
            }
        }*/
        
    }
}

void Inventory::update()
{
    for (auto& ing : items)
    {
        if (ing->isDragging)
        {
            ing->sprite.setPosition(currentMousePos + ing->dragOffset);
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

void Inventory::setMousePosition(sf::Vector2f pos)
{
    currentMousePos = pos;
}

std::unique_ptr<Ingredient> Inventory::takeDraggedItem()
{
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i]->isDragging)
        {
            auto ptr = std::move(items[i]); // take ownership
            items.erase(items.begin() + i);

            // realign remaining items
            for (size_t j = 0; j < items.size(); j++)
            {
                items[j]->sprite.setPosition({
                    slots[j].getPosition().x + slotSize / 2.f,
                    slots[j].getPosition().y + slotSize / 2.f
                    });
            }

            return ptr; //  return moved item
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