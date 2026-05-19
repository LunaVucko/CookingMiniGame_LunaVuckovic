#include "Ladle.h"

Ladle::Ladle(sf::Texture& texture,
    sf::IntRect emptyRect,
    sf::IntRect filledRect)
    : Item(texture)
{
    this->emptyRect = emptyRect;
    this->filledRect = filledRect;

    sprite.setTexture(texture);
    sprite.setTextureRect(emptyRect);
}

void Ladle::updateSprite()
{
    if (state == LadleState::Empty)
        sprite.setTextureRect(emptyRect);
    else
        sprite.setTextureRect(filledRect);
}