#include "TutorialState.h"
#include "StateManager.h"
#include "PlayState.h"

#include <iostream>

TutorialState::TutorialState(StateManager& manager, std::vector<std::string> images)
    : manager(manager), pages(std::move(images)), nextText(font)
{
    manager.pauseTimer = true;
    manager.showTimer = false;

    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    nextButton.setSize({ 180.f, 60.f });
    nextButton.setPosition({ 740.f, 620.f });
    nextButton.setFillColor(sf::Color(50, 50, 50));

    nextText.setFont(font);
    nextText.setCharacterSize(28);
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

    loadPage(0);
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

    if (!nextButton.getGlobalBounds().contains(mousePos))
        return;

    // LAST PAGE and then go to PLAY
    if (currentPage == (int)pages.size() - 1)
    {
        manager.setState(std::make_unique<PlayState>(manager));
        return;
    }

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
}