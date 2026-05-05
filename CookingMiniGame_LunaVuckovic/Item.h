#pragma once
#include <SFML/Graphics.hpp>

struct Item
{
    sf::Sprite sprite;

    bool isDragging = false;
    sf::Vector2f dragOffset;

    Item(sf::Texture& texture)
        : sprite(texture)  
    {
    }

    virtual ~Item() = default;

    // might use it for later
    virtual void update() {}
};