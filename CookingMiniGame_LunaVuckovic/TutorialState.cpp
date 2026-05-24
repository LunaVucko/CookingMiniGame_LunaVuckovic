#include "TutorialState.h"
#include "StateManager.h"
#include "PlayState.h"

#include <iostream>

TutorialState::TutorialState(StateManager& manager, std::vector<std::string> images)
    : manager(manager), pages(std::move(images)), nextText(font), skipText(font)
{
    manager.pauseTimer = true;
    manager.showTimer = false;

    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    //next button
    nextButton.setSize({ 150.f, 50.f });
    nextButton.setPosition({ 780.f, 650.f });
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

    // skip button

    skipButton.setSize({ 150.f, 50.f });
    skipButton.setPosition({ 780.f, 590.f });
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
    
    //next button

    if (!nextButton.getGlobalBounds().contains(mousePos))
    { 
        return;
    }


    // LAST PAGE and then go to PLAY
    if (currentPage == (int)pages.size() - 1)
    {
        pageSound.value().play();

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
}

void TutorialState::draw(sf::RenderWindow& window)
{
    window.draw(image);
    window.draw(nextButton);
    window.draw(nextText);
    window.draw(skipButton);
    window.draw(skipText);
}