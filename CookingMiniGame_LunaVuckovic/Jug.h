#pragma once
#include "Item.h"
#include <SFML/Graphics.hpp>

enum class JugState
{
    Empty,
    Filled
};

struct Jug : public Item
{
    JugState state = JugState::Empty;

    sf::IntRect emptyRect;
    sf::IntRect filledRect;

    Jug(sf::Texture& texture,
        const sf::IntRect& empty,
        const sf::IntRect& filled)
        : Item(texture),
        emptyRect(empty),
        filledRect(filled)
    {
        sprite.setTextureRect(emptyRect);
    }

    void updateSprite()
    {
        if (state == JugState::Empty)
        {
            sprite.setTextureRect(emptyRect);
        }
        else
        {
            sprite.setTextureRect(filledRect);
        }
    }
};