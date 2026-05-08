#pragma once
#include "Item.h"
#include <SFML/Graphics.hpp>
#include "CookingItem.h"

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

    std::vector<CookingItem> stoveItems;

    CookingItem* activeCookingItem = nullptr;

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