#include "PlayState.h"
#include "StoveState.h"
#include "SinkState.h"
#include "KitchenCounterState.h"
#include "MenuState.h"
#include <iostream>
#include "TutorialState.h"

PlayState::PlayState(StateManager& manager) : manager(manager), cookbookText(font)
{
    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    //cookbookbutton
    cookbookButton.setSize({ 150.f, 50.f });
    cookbookButton.setPosition({ 350.f,150.f });
    cookbookButton.setFillColor(sf::Color(34, 139, 34));

 

    cookbookText.setFont(font);
    cookbookText.setCharacterSize(22);
    cookbookText.setFillColor(sf::Color::White);
    cookbookText.setString("CookBook");

    sf::FloatRect bounds = cookbookText.getLocalBounds();
    cookbookText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    cookbookText.setPosition({
         cookbookButton.getPosition().x + cookbookButton.getSize().x / 2.f,
           cookbookButton.getPosition().y + cookbookButton.getSize().y / 2.f
        });

    if (!texture.loadFromFile("Texture/game_photo.png"))
    {
        std::cout << "Failed to load kitchen background\n";
    }
    if (!sinkHoverTexture.loadFromFile("Texture/game_photo_sink.png"))
    {
        std::cout << "Failed to load selected sink kitchen background\n";
    }
    if (!stoveHoverTexture.loadFromFile("Texture/game_photo_stove.png"))
    {
        std::cout << "Failed to load selected stove kitchen background\n";
    }
    if (!counterHoverTexture.loadFromFile("Texture/game_photo_cutting_board.png"))
    {
        std::cout << "Failed to load selected cutting board kitchen background\n";
    }

    manager.pauseTimer = false;
    manager.showTimer = true;

    manager.gameTimer.restart();
    manager.lastFrameTime = manager.gameTimer.getElapsedTime().asSeconds();

    background.setSize({ 960, 720 });
    background.setTexture(&texture);


    float inventoryHeight = 100.f;

    stoveArea = sf::FloatRect({ 220.f, 0.f }, { 420.f, 720.f - inventoryHeight}); //position of where to click the stove in the full kitchen layout
    kitchenCounterArea = sf::FloatRect({ 640.f, 0.f }, { 320.f, 720.f - inventoryHeight}); //position of where to click the counter in the full kitchen layout
    sinkArea = sf::FloatRect({ 0.f, 0.f }, { 220.f, 720.f - inventoryHeight}); //position of where to click the sink in the full kitchen layout

   
}

void PlayState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    manager.inventory.handleEvent(event);

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        mousePos =
        {
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        };

        mouseInitialized = true;
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                static_cast<float>(mouse->position.x),
                static_cast<float>(mouse->position.y)
            );
            
            if (manager.inventory.contains(mousePos))
                return;

            if (cookbookButton.getGlobalBounds().contains(mousePos))
            {
                manager.setState(std::make_unique<TutorialState>(
                    manager,
                    manager.tutorialPages,
                    TutorialReturn::Play));

                return;
            }

            if (sinkArea.contains(mousePos))
            {
                nextState = std::make_unique<SinkState>(manager);
            }
            else if (stoveArea.contains(mousePos))
            {
                nextState = std::make_unique<StoveState>(manager);
            }
            else if (kitchenCounterArea.contains(mousePos))
            {
                nextState = std::make_unique<KitchenCounterState>(manager);
            }
        }
    }

    //escape back to menu
    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.timerStarted = false;
            manager.showTimer = false;
            manager.gameTimer.restart();
            nextState = std::make_unique<MenuState>(manager, 960, 720);
        }
    }
}

void PlayState::update() 
{
    if (!mouseInitialized)
    {
        background.setTexture(&texture);
    }
    else if (sinkArea.contains(mousePos))
    {
        background.setTexture(&sinkHoverTexture);
    }
    else if (stoveArea.contains(mousePos))
    {
        background.setTexture(&stoveHoverTexture);
    }
    else if (kitchenCounterArea.contains(mousePos))
    {
        background.setTexture(&counterHoverTexture);
    }
    else
    {
        background.setTexture(&texture);
    }

    if (nextState)
    {
        manager.setState(std::move(nextState));
    }
}

void PlayState::draw(sf::RenderWindow& window)
{

 
    window.draw(background);

    //inventory

    manager.inventory.draw(window);


    
    sf::RectangleShape debugRect;

    window.draw(cookbookButton);
    window.draw(cookbookText);
   
   

    /*
    *     //Sink (Left - blue)
    debugRect.setPosition({ 0.f, 0.f });
    debugRect.setSize({ 320.f, 720.f });
    debugRect.setFillColor(sf::Color(0, 0, 225, 100));
    window.draw(debugRect);

    //Stove (Middle - red)
    debugRect.setPosition({ 320.f, 0.f });
    debugRect.setFillColor(sf::Color(255, 0, 0, 100));
    window.draw(debugRect);


    //Counter (Right - green)
    debugRect.setPosition({ 640.f, 0.f });
    debugRect.setFillColor(sf::Color(0, 255, 0, 100));
    window.draw(debugRect);
    */

    
}