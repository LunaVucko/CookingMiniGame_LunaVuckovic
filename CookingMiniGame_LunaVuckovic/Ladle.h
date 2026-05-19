#pragma once
#include "Item.h"

enum class LadleState
{
    Empty,
    Filled
};

class Ladle : public Item
{
public:
    LadleState state = LadleState::Empty;

    sf::IntRect emptyRect;
    sf::IntRect filledRect;

    Ladle(sf::Texture& texture,
        sf::IntRect emptyRect,
        sf::IntRect filledRect);

    void updateSprite();
};