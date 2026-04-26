#pragma once

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

inline bool canPeel(IngredientType type)
{
    switch (type)
    {
    case IngredientType::Carrot:
    case IngredientType::Parsnip:
    case IngredientType::Cereliac:
    case IngredientType::Garlic:
        return true;

    default:
        return false;
    }
}

inline bool canCut(IngredientType type)
{
    switch (type)
    {
    case IngredientType::Garlic: 
        return false;

    default:
        return true;
    }
}