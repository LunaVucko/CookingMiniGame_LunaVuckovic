#include "Bowl.h"

Bowl::Bowl(sf::Texture& texture,
    sf::IntRect emptyRect,
    sf::IntRect halfRect,
    sf::IntRect fullRect)
    : Item(texture)
{
    this->emptyRect = emptyRect;
    this->halfRect = halfRect;
    this->fullRect = fullRect;

    sprite.setTexture(texture);
    sprite.setTextureRect(emptyRect);
}

void Bowl::addSoup()
{
    if (soupAmount < maxSoup)
        soupAmount++;

    updateState();
}

bool Bowl::isFull() const
{
    return soupAmount >= maxSoup;
}

void Bowl::updateState()
{
    if (soupAmount == 0)
        state = BowlState::Empty;
    else if (soupAmount < maxSoup)
        state = BowlState::HalfFull;
    else
        state = BowlState::Full;

    updateSprite();
}

void Bowl::updateSprite()
{
    switch (state)
    {
    case BowlState::Empty:
        sprite.setTextureRect(emptyRect);
        break;
    case BowlState::HalfFull:
        sprite.setTextureRect(halfRect);
        break;
    case BowlState::Full:
        sprite.setTextureRect(fullRect);
        break;
    }
}