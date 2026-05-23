#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "GameState.h"

class StateManager;

class TutorialState : public GameState
{
public:
    TutorialState(StateManager& manager, std::vector<std::string> images);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
    void update();
    void draw(sf::RenderWindow& window);

private:
    StateManager& manager;

    sf::Texture texture;
    sf::RectangleShape image;

    sf::Font font;

    sf::RectangleShape nextButton;
    sf::Text nextText;

    std::vector<std::string> pages;
    int currentPage = 0;

    void loadPage(int index);

    //sfx
       //sfx

    sf::SoundBuffer pageBuffer;


    std::optional<sf::Sound> pageSound;
  
};