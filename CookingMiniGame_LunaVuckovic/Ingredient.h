#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

struct Ingredient
{
    sf::Sprite sprite;

    bool isDragging = false;
   
    Ingredient(sf::Texture& texture, const sf::IntRect& rect) : sprite(texture)
    {

        sprite.setTextureRect(rect);
    }
};
