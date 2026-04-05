#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

struct Ingredient
{
    sf::Sprite sprite;
    sf::Texture texture;

    bool isDragging = false;

    Ingredient(const string& path) : texture(), sprite()
    {
        if (!texture.loadFromFile(path))
        {
            cout << "Failed to load: " << path << endl;
        }

        sprite.setTexture(texture);
    }
};
