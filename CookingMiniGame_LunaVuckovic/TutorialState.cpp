#include "TutorialState.h"
#include "StateManager.h"
#include "PlayState.h"

#include <iostream>

TutorialState::TutorialState(StateManager& manager, std::vector<std::string> images)
    : manager(manager), pages(std::move(images)), nextText(font), skipText(font), previousText(font)
{
    manager.pauseTimer = true;
    manager.showTimer = false;

    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    //next button
    nextButton.setSize({ 150.f, 50.f });
    nextButton.setPosition({ 780.f, 610.f });
    nextButton.setFillColor(sf::Color(186, 142, 35));

    nextText.setFont(font);
    nextText.setCharacterSize(22);
    nextText.setFillColor(sf::Color::White);
    nextText.setString("Next");

    sf::FloatRect bounds = nextText.getLocalBounds();
    nextText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    nextText.setPosition({
        nextButton.getPosition().x + nextButton.getSize().x / 2.f,
        nextButton.getPosition().y + nextButton.getSize().y / 2.f
        });

    //previous button

    previousButton.setSize({ 150.f, 50.f });
    previousButton.setPosition({ 780.f, 550.f });
    previousButton.setFillColor(sf::Color(144, 213, 255));

    previousText.setFont(font);
    previousText.setCharacterSize(22);
    previousText.setFillColor(sf::Color::White);
    previousText.setString("Previous");

    sf::FloatRect previousBounds = previousText.getLocalBounds();

    previousText.setOrigin({
        previousBounds.position.x + previousBounds.size.x / 2.f,
        previousBounds.position.y + previousBounds.size.y / 2.f
        });

    previousText.setPosition({
        previousButton.getPosition().x + previousButton.getSize().x / 2.f,
        previousButton.getPosition().y + previousButton.getSize().y / 2.f
        });

    // skip button

    skipButton.setSize({ 150.f, 50.f });
    skipButton.setPosition({ 780.f, 490.f });
    skipButton.setFillColor(sf::Color(34, 139, 34));

    skipText.setFont(font);
    skipText.setCharacterSize(22);
    skipText.setFillColor(sf::Color::White);
    skipText.setString("Skip");

    sf::FloatRect skipBounds = skipText.getLocalBounds();

    skipText.setOrigin({
        skipBounds.position.x + skipBounds.size.x / 2.f,
        skipBounds.position.y + skipBounds.size.y / 2.f
        });

    skipText.setPosition({
        skipButton.getPosition().x + skipButton.getSize().x / 2.f,
        skipButton.getPosition().y + skipButton.getSize().y / 2.f
        });


    loadPage(0);

    //sfx

    if (!pageBuffer.loadFromFile("SFX/page.wav"))
        std::cout << "Failed to load page sound\n";


    pageSound.emplace(pageBuffer);
  
    pageSound.value().setVolume(50.f);

    if (!skipBuffer.loadFromFile("SFX/skip.wav"))
        std::cout << "Failed to load skip sound\n";


    skipSound.emplace(skipBuffer);

    skipSound.value().setVolume(50.f);
}

void TutorialState::loadPage(int index)
{
    if (index < 0 || index >= (int)pages.size())
        return;

    if (!texture.loadFromFile(pages[index]))
    {
        std::cout << "Failed to load tutorial image: " << pages[index] << "\n";
    }

    image.setSize({ 960.f, 720.f });
    image.setTexture(&texture);
    image.setPosition({ 0.f, 0.f });

    // Change button text on the last page
    if (index == (int)pages.size() - 1)
    {
        nextText.setString("Cook!");
    }
    else
    {
        nextText.setString("Next");
    }

    // Re-center the text because the width changed
    sf::FloatRect bounds = nextText.getLocalBounds();

    nextText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    nextText.setPosition({
        nextButton.getPosition().x + nextButton.getSize().x / 2.f,
        nextButton.getPosition().y + nextButton.getSize().y / 2.f
        });
}

void TutorialState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (!event.is<sf::Event::MouseButtonPressed>())
        return;

    auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

    sf::Vector2f mousePos(
        (float)mouse->position.x,
        (float)mouse->position.y
    );

    //skip button

    if (skipButton.getGlobalBounds().contains(mousePos))
    {
        skipSound.value().play();
        sf::sleep(sf::milliseconds(650));
        manager.setState(std::make_unique<PlayState>(manager));
        return;
    }

    // Previous button

    if (previousButton.getGlobalBounds().contains(mousePos))
    {
        if (currentPage > 0)
        {
            pageSound.value().play();
            sf::sleep(sf::milliseconds(450));

            currentPage--;
            loadPage(currentPage);
        }

        return;
    }
    
    //next button

    if (!nextButton.getGlobalBounds().contains(mousePos))
    { 
        return;
    }


    // LAST PAGE and then go to PLAY
    if (currentPage == (int)pages.size() - 1)
    {
        manager.enterSound.value().play();

        // wait a tiny bit before switching state
        sf::sleep(sf::milliseconds(650));

        manager.setState(std::make_unique<PlayState>(manager));
        return;
    }
    pageSound.value().play();
    sf::sleep(sf::milliseconds(450));

    // NEXT PAGE
    currentPage++;
    loadPage(currentPage);
}

void TutorialState::update()
{
    if (currentPage == 0)
    {
        previousButton.setFillColor(sf::Color(120, 120, 120));
    }
    else
    {
        previousButton.setFillColor(sf::Color(70, 130, 180));
    }
}

void TutorialState::draw(sf::RenderWindow& window)
{
    window.draw(image);
    window.draw(previousButton);
    window.draw(previousText);
    window.draw(nextButton);
    window.draw(nextText);
    window.draw(skipButton);
    window.draw(skipText);
}