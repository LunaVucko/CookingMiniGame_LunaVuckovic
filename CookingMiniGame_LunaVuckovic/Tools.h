#pragma once
#include <SFML/Graphics.hpp>

enum class ToolType
{
    None,
    Peeler,
    Knife
};

struct Tools
{
    sf::Sprite sprite;
    ToolType type;

    Tools(sf::Texture& texture, const sf::IntRect& rect, ToolType t)
        : sprite(texture), type(t)
    {
        sprite.setTextureRect(rect);
    }
};