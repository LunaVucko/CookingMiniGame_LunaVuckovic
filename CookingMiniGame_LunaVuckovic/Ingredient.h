#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

struct Ingredient
{
    sf::Sprite sprite;
    sf::Texture texture;

    bool isDragging = false;
    bool isLoaded = false;

    Ingredient(const string& path) : texture(), sprite(texture)
    {
        if (!texture.loadFromFile(path))
        {
            cout << "Failed to load: " << path << endl;
            isLoaded = false;
        }
        else 
        {
            cout << "Load successfully: " << path
            << " | Size: "
            << texture.getSize().x << "x"
            << texture.getSize().y << std::endl;
            sprite.setTexture(texture);
            isLoaded = true;

        }
    }
};
