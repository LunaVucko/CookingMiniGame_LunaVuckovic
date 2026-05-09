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

    sf::IntRect stoveEmptyRect;
    sf::IntRect stoveFilledRect;

    std::vector<CookingItem> stoveItems;

    CookingItem* activeCookingItem = nullptr;

    PotState state = PotState::Empty;

    bool isOnStove = false;

    //bool isDragging = false;
    //sf::Vector2f dragOffset;

    Pot(sf::Texture& texture, const sf::IntRect& empty, const sf::IntRect& filled, const sf::IntRect& stoveEmpty,
        const sf::IntRect& stoveFilled) : Item(texture), emptyRect(empty), filledRect(filled), stoveEmptyRect(stoveEmpty),stoveFilledRect(stoveFilled)
    {
        sprite.setTextureRect(emptyRect);
    }

    void updateSprite()
    {
        ///top view sprites
        if (isOnStove)
        {
            if (state == PotState::Empty)
            {
                sprite.setTextureRect(stoveEmptyRect);
            }
            else
            {
                sprite.setTextureRect(stoveFilledRect);

            }
        }
        //side view sprites
        else
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
    }
};