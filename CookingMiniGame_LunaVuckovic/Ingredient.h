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

    IngredientType type;
    IngredientState state = IngredientState::Whole;

    bool isDragging = false;
   
    Ingredient(sf::Texture& texture, const sf::IntRect& rect, IngredientType t) : sprite(texture), type(t)
    {

        sprite.setTextureRect(rect);
    }
};
