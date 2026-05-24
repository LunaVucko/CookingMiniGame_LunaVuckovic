#include "ResultState.h"
#include "MenuState.h"
#include "StateManager.h"

#include <iostream>

ResultState::ResultState(StateManager& manager, ResultType type)
    : manager(manager), resultType(type),
    resultText(font),
    scoreText(font),
    continueText(font)
{

    manager.stopGameMusic();
    //music stopped 

    manager.showTimer = false;

   // ResultType resultType;

    // background
    background.setSize({ 960.f, 720.f });
    background.setFillColor(sf::Color(30, 30, 30));

    // load font
    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    // RESULT TEXT
    resultText.setFont(font);
    resultText.setCharacterSize(60);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({ 300.f, 150.f });

    // SCORE TEXT
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 360.f, 300.f });

    // CONTINUE TEXT
    continueText.setFont(font);
    continueText.setCharacterSize(30);
    continueText.setFillColor(sf::Color::Yellow);
    continueText.setPosition({ 230.f, 500.f });

    continueText.setString("Click anywhere to return to menu");

    //timer running out
    if (resultType == ResultType::TimeUp)
    {
        manager.tryAgainSound.value().play();
        resultText.setString("TIME'S UP!");
        scoreText.setString("You ran out of time\nTry again!");
    }
    else {


        // determine ranking
        if (manager.soupScore < 40)
        {
            //manager.tryAgainSound.value().play();
            resultText.setString("TRY AGAIN");
        }
        else if (manager.soupScore < 70)
        {
            //manager.goodSound.value().play();
            resultText.setString("GOOD");
        }
        else
        {
            //manager.amazingSound.value().play();
            resultText.setString("AMAZING");
        }

       // scoreText.setString("Final Score: " + std::to_string(manager.soupScore));
    }

    // center result text
    sf::FloatRect bounds = resultText.getLocalBounds();

    resultText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    resultText.setPosition({ 510.f, 180.f });

    if (resultType != ResultType::TimeUp)
    {
        // score text
        scoreText.setString(
            "Final Score: " + std::to_string(manager.soupScore)
        );
    }

    sf::FloatRect scoreBounds = scoreText.getLocalBounds();

    scoreText.setOrigin({
        scoreBounds.position.x + scoreBounds.size.x / 2.f,
        scoreBounds.position.y + scoreBounds.size.y / 2.f
        });

    scoreText.setPosition({ 480.f, 350.f });
}

void ResultState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            // RESET GAME DATA
            manager.resetGame();

            // RETURN TO MENU
            manager.setState(std::make_unique<MenuState>(manager, 960.f, 720.f));
        }
    }
}

void ResultState::update()
{
}

void ResultState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    window.draw(resultText);

    window.draw(scoreText);

    window.draw(continueText);
}