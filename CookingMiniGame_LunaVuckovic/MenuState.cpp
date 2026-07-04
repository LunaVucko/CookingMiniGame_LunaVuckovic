#include "MenuState.h"
#include "StateManager.h"
#include "PlayState.h"
#include "OptionsState.h"
#include "TutorialState.h"

void MenuState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{
    if (event.is<sf::Event::KeyReleased>())
    {
        auto key = event.getIf<sf::Event::KeyReleased>();

        if (key->code == sf::Keyboard::Key::Up)
        {
            manager.selectSound.value().play();
            menu.MoveUp();
        }
            

        if (key->code == sf::Keyboard::Key::Down)
        {
            manager.selectSound.value().play();
            menu.MoveDown();
        }
            

        if (key->code == sf::Keyboard::Key::Enter)
        {
            int x = menu.MainMenuPressed();

            if (x == 0)
            {
                manager.soupScore = 0;
                //manager.gameTimer.restart();

                manager.totalPausedTime = 0.f;
                manager.pausedAt = 0.f;
                manager.wasPaused = false;

                manager.elapsedTime = 0.f;
                manager.lastFrameTime = manager.gameTimer.getElapsedTime().asSeconds();

                manager.enterSound.value().play();
                sf::sleep(sf::milliseconds(850));

               // manager.setState(std::make_unique<PlayState>(manager)); // PLAY
                manager.setState(std::make_unique<TutorialState>(
                    manager,
                    manager.tutorialPages,
                    TutorialReturn::StartGame
                ));
            }

            if (x == 1)
            {
                manager.setState(std::make_unique<OptionsState>(manager)); //OPTIONS
            }

       
            if (x == 2)
            {
                window.close(); // EXIT
            }
               
        }
    }
}