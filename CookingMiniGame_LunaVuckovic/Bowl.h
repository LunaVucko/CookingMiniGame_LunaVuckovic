#pragma once
#include "Item.h"

enum class BowlState
{
    Empty,
    HalfFull,
    Full
};

class Bowl : public Item
{
public:
    BowlState state = BowlState::Empty;

    int soupAmount = 0;
    int maxSoup = 2;

    sf::IntRect emptyRect;
    sf::IntRect halfRect;
    sf::IntRect fullRect;

    Bowl(sf::Texture& texture,
        sf::IntRect emptyRect,
        sf::IntRect halfRect,
        sf::IntRect fullRect);

    void addSoup();
    bool isFull() const;
    void updateState();
    void updateSprite();
};