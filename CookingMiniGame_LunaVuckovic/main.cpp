#include <SFML/Graphics.hpp>
#include "StateManager.h"
#include "MenuState.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 960, 720 }), "Game");

    StateManager manager;
    manager.setState(std::make_unique<MenuState>(manager, 960, 720));

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            manager.handleEvent(window, *event);
        }

        manager.update();

        window.clear();
        manager.draw(window);
        window.display();
    }
}