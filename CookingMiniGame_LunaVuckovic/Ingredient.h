#pragma once
#include <SFML/Graphics.hpp>

struct Ingredient
{
    sf::Sprite sprite;
    sf::Texture texture;

    bool isDragging = false;

    Ingredient(const std::string& path)
    {
        texture.loadFromFile(path);
        sprite.setTexture(texture);
    }
};