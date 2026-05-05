#pragma once
#include "Item.h"
#include <SFML/Graphics.hpp>

enum class PotState
{
    Empty,
    Filled
};

struct Pot : public Item
{
    //sf::Sprite sprite;

    sf::IntRect emptyRect;
    sf::IntRect filledRect;

    PotState state = PotState::Empty;

    //bool isDragging = false;
    //sf::Vector2f dragOffset;

    Pot(sf::Texture& texture, const sf::IntRect& empty, const sf::IntRect& filled) : Item(texture), emptyRect(empty), filledRect(filled)
    {
        sprite.setTextureRect(emptyRect);
    }

    void updateSprite()
    {
        if (state == PotState::Empty)
        { 
            sprite.setTextureRect(emptyRect);
        }
        else
        { 
            sprite.setTextureRect(filledRect);
        }
    }
};