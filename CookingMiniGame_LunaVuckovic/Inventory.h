#pragma once
#include <SFML/Graphics.hpp>
#include "Ingredient.h"
#include <vector>
#include <memory>

enum class InventoryType
{
    Top,
    Bottom
};

class Inventory
{
private:
    std::vector<std::unique_ptr<Ingredient>> items;

    sf::RectangleShape inventoryBar;
    std::vector<sf::RectangleShape> slots;

    //sf::Texture ingredientsTexture;

    float slotSize = 80.f;
    float yOffset;

public:
    Inventory(InventoryType type);

    void setupSlots();
    void addItem(std::unique_ptr<Ingredient> item);

    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    Ingredient* getDraggedItem();
    std::unique_ptr<Ingredient> takeDraggedItem();

    bool contains(sf::Vector2f point) const;

    bool isEmpty() const;
};