#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "GameState.h"

enum class TutorialReturn
{
    StartGame,
    Play,
    Sink,
    Stove,
    Counter,
    Plating
};

class StateManager;

class TutorialState : public GameState
{
public:
    TutorialState(StateManager& manager, std::vector<std::string> images, TutorialReturn mode = TutorialReturn::StartGame);

    void handleEvent(sf::RenderWindow& window, const sf::Event& event);
    void update();
    void draw(sf::RenderWindow& window);

private:
    StateManager& manager;

    TutorialReturn returnMode;

    sf::Texture texture;
    sf::RectangleShape image;

    sf::Font font;

    sf::RectangleShape nextButton;
    sf::Text nextText;

    sf::RectangleShape previousButton;
    sf::Text previousText;

    sf::RectangleShape skipButton;
    sf::Text skipText;

    std::vector<std::string> pages;
    int currentPage = 0;

    void loadPage(int index);

    //sfx

    sf::SoundBuffer pageBuffer;
    std::optional<sf::Sound> pageSound;

    sf::SoundBuffer skipBuffer;
    std::optional<sf::Sound> skipSound;
  
};