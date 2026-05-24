#include "PlatingState.h"
#include "PlayState.h"
#include "ResultState.h"
#include <iostream>

PlatingState::PlatingState(StateManager& manager)
    : manager(manager)
{
    background.setSize({ 960.f, 720.f });
    background.setTexture(&manager.platingTexture);


    // CREATE POT
    if (!pot)
    {
        pot = manager.createPot();

        pot->sprite.setPosition({ 380.f, 100.f });
        pot->sprite.setScale({ 0.6f, 0.6f });
    }

    // CREATE BOWL
    if (!bowl)
    {
        bowl = manager.createBowl();
        bowl->sprite.setPosition({ 200.f, 400.f });
        bowl->sprite.setScale({ 0.6f, 0.6f });
    }

    // CREATE LADLE
    if (!ladle)
    {
        ladle = manager.createLadle();
        ladle->sprite.setPosition({ 400.f, 300.f });
        ladle->sprite.setScale({ 0.6f, 0.6f });
    }

}
void PlatingState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            manager.setState(std::make_unique<PlayState>(manager));
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (mouse->button == sf::Mouse::Button::Left)
        {
           
            if (ladle && ladle->sprite.getGlobalBounds().contains(mousePos))
            {
                ladle->isDragging = true;
                ladle->dragOffset = ladle->sprite.getPosition() - mousePos;
                std::cout << "[DEBUG] Started dragging ladle\n";
            }
        }
    }

    if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        currentMousePos = {
            (float)mouse->position.x,
            (float)mouse->position.y
        };
    }

    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        if (!ladle || !pot || !bowl)
            return;


        if (!ladle->isDragging)
            return;

        sf::FloatRect potArea = pot->sprite.getGlobalBounds();

        sf::FloatRect bowlArea = bowl->sprite.getGlobalBounds();

        if (ladle && ladle->isDragging)
        {
            ladle->isDragging = false;

           
                // 1. FILL THE LADL WITH SOUP FROM POT
                if (potArea.contains(mousePos))
                {
                    std::cout << "[DEBUG] Dropped on POT area\n";

                    if (ladle->state == LadleState::Empty)
                    {
                        ladle->state = LadleState::Filled;
                        ladle->updateSprite();

                        manager.pourSound.value().play();
                        std::cout << "Ladle filled from pot\n";
                    }
                    else
                    {
                        manager.wrongBuzzSound.value().play();
                        std::cout << "Ladle already filled!\n";
                    }

                    return;
                }
                // 2. POUR INTO BOWL
                if (bowlArea.contains(mousePos))
                {
                    std::cout << "[DEBUG] Dropped on BOWL area\n";

                    if (ladle->state == LadleState::Filled)
                    {
                        ladle->state = LadleState::Empty;
                        ladle->updateSprite();

                        bowl->addSoup();
                        bowl->updateSprite();

                        manager.pourQuickSound.value().play();

                        std::cout << "Soup transferred to bowl\n";
                        std::cout << "[DEBUG] Bowl soup amount increased\n";

                        if (bowl->isFull())
                        {
                            manager.setState(std::make_unique<ResultState>(
                                manager,
                                ResultType::Score
                            ));
                        }
                    }
                    else
                    {
                        manager.wrongBuzzSound.value().play();
                        std::cout << "Ladle empty!\n";
                    }

                    return;
                }

                std::cout << "[DEBUG] Dropped on EMPTY AREA\n";
            
        }

        
    }
}

void PlatingState::update()
{

    if (ladle && ladle->isDragging)
    {
        ladle->sprite.setPosition(currentMousePos + ladle->dragOffset);
    }


}

void PlatingState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    if (pot)
    {

        window.draw(pot->sprite);

        sf::FloatRect potBounds = pot->sprite.getGlobalBounds();
        /*
        *      sf::RectangleShape potDebug;
        potDebug.setPosition(potBounds.position);
        potDebug.setSize(potBounds.size);
        potDebug.setFillColor(sf::Color(0, 255, 0, 80));

        window.draw(potDebug);
        */
       
    }

    if (bowl)
    {
        window.draw(bowl->sprite);



        sf::FloatRect bowlBounds = bowl->sprite.getGlobalBounds();

    /*    sf::RectangleShape bowlDebug;
        bowlDebug.setPosition(bowlBounds.position);
        bowlDebug.setSize(bowlBounds.size);
        bowlDebug.setFillColor(sf::Color(0, 0, 255, 80));

        window.draw(bowlDebug);*/
    }
    
    if (ladle)
    {
        window.draw(ladle->sprite);
    }

}