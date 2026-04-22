#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

enum class IngredientType
{
    Carrot,
    Parsnip,
    Chicken,
    Cereliac,
    Garlic,
    Buillion,
    Parsley
};

enum class IngredientState
{
    Whole,
    Peeled,
    Cut
};

struct Ingredient
{
    sf::Sprite sprite;

    sf::IntRect wholeRect;
    sf::IntRect peeledRect;
    sf::IntRect cutRect;

    IngredientType type;
    IngredientState state = IngredientState::Whole;

    bool isDragging = false;
   
    Ingredient(sf::Texture& texture, const sf::IntRect& whole, const sf::IntRect& peeled, const sf::IntRect& cut, IngredientType t) : sprite(texture), wholeRect(whole), peeledRect(peeled), cutRect(cut), type(t)
    {

        sprite.setTextureRect(wholeRect);
    }

    void updateSprite()
    {
        if (state == IngredientState::Whole)
            sprite.setTextureRect(wholeRect);
        else if (state == IngredientState::Peeled)
            sprite.setTextureRect(peeledRect);
        else if (state == IngredientState::Cut)
            sprite.setTextureRect(cutRect);
    }
};
