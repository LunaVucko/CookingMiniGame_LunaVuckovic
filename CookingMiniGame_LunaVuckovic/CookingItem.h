#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Item.h"


enum class CookState
{
    Raw,
    Cooked,
    Overcooked
};

struct CookingItem
{
    std::unique_ptr<Item> item;

    CookState cookState = CookState::Raw;

    sf::IntRect rawRect;
    sf::IntRect cookedRect;
    sf::IntRect overcookedRect;

    sf::Clock cookingClock;

    bool isCookingStarted = false;

    float cookTime = 5.f;
    float burnTime = 10.f;
};