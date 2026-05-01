#pragma once
#include <SFML/Graphics.hpp>

enum class PotState
{
    Empty,
    Filled
};

struct Pot
{
    sf::Sprite sprite;

    sf::IntRect emptyRect;
    sf::IntRect filledRect;

    PotState state = PotState::Empty;

    bool isDragging = false;
    sf::Vector2f dragOffset;

    Pot(sf::Texture& texture,
        const sf::IntRect& empty,
        const sf::IntRect& filled)
        : sprite(texture), emptyRect(empty), filledRect(filled)
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
            sprite.setTextureRect(filledRect);
    }
};