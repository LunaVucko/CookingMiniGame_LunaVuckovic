#include "MenuState.h"
#include "StateManager.h"
#include "PlayState.h"
#include "OptionsState.h"

void MenuState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyReleased>())
    {
        auto key = event.getIf<sf::Event::KeyReleased>();

        if (key->code == sf::Keyboard::Key::Up)
            menu.MoveUp();

        if (key->code == sf::Keyboard::Key::Down)
            menu.MoveDown();

        if (key->code == sf::Keyboard::Key::Enter)
        {
            int x = menu.MainMenuPressed();

            if (x == 0)
                manager.setState(std::make_unique<PlayState>(manager)); // PLAY

            if (x == 1)
                manager.setState(std::make_unique<OptionsState>(manager)); //OPTIONS

            if (x == 2)
                window.close(); // EXIT
        }
    }
}