#include <SFML/Graphics.hpp>
#include "MainMenu.h"
using namespace sf;

int main()
{
    //Main Window

    RenderWindow MENU(VideoMode({ 960, 720 }), "Main Menu", Style::Default);
    MainMenu mainMenu(MENU.getSize().x, MENU.getSize().y);

    // set background
    RectangleShape background;
    background.setSize(Vector2f(960, 720));
    Texture MainTexture;
    MainTexture.loadFromFile("Texture/background.png");
    background.setTexture(&MainTexture);
    
    //Photo for the game
    RectangleShape PhotoBackground;
    PhotoBackground.setSize(Vector2f(960, 720));
    Texture back_texture;
    back_texture.loadFromFile("Texture/game_photo.png");
    PhotoBackground.setTexture(&back_texture);

    //Photo for the option

    RectangleShape OptionBackground;
    OptionBackground.setSize(Vector2f(960, 720));
    Texture OptionTexture;
    OptionTexture.loadFromFile("Texture/option_photo.png");
    OptionBackground.setTexture(&OptionTexture);




    while (MENU.isOpen())
    {
        while (auto event = MENU.pollEvent())
        {
            if (event->is < sf::Event::Closed>())
            {
                MENU.close();
            }
            if (event->is<Event::KeyReleased>())
            {
                if (event->getIf<sf::Event::KeyReleased>() && event->getIf<sf::Event::KeyReleased>()->code == Keyboard::Key::Up)
                {
                    mainMenu.MoveUp();
                }
                if (event->getIf<sf::Event::KeyReleased>() && event->getIf<sf::Event::KeyReleased>()->code == Keyboard::Key::Down)
                {
                    mainMenu.MoveDown();
                }
                if (event->getIf<sf::Event::KeyReleased>() && event->getIf<sf::Event::KeyReleased>()->code == Keyboard::Key::Enter)
                {
                    RenderWindow Play(VideoMode({ 920,720 }), "game_name");
                    RenderWindow OPTIONS(VideoMode({ 920,720 }), "OPTIONS");

                    int x = mainMenu.MainMenuPressed();
                    if (x == 0)
                    {
                        while (Play.isOpen())
                        {
                            while (auto event = Play.pollEvent())
                            {
                                if (event->is < sf::Event::Closed>())
                                {
                                    Play.close();
                                }
                                if (event->is<Event::KeyPressed>())
                                {
                                    if (event->getIf<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == Keyboard::Key::Escape)
                                    {
                                        Play.close();
                                    }
                                }
                            }
                            OPTIONS.close();
                            Play.clear();

                            Play.draw(PhotoBackground);
                            Play.display();
                        }

                    }
                    if (x == 1)
                    {
                        while (OPTIONS.isOpen())
                        {
                            while (auto event = OPTIONS.pollEvent())
                            {
                                if (event->is < sf::Event::Closed>())
                                {
                                    OPTIONS.close();
                                }
                                if (event->is<Event::KeyPressed>())
                                {
                                    if (event->getIf<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == Keyboard::Key::Escape)
                                    {
                                        OPTIONS.close();
                                    }
                                }
                            }
                            Play.close();
                            OPTIONS.clear();
                            
                            OPTIONS.draw(OptionBackground);
                            OPTIONS.display();
                        }
                    }
                    if (x == 2)
                    {
                        MENU.close();
                    }

                }
            }
        }


        MENU.clear();
        MENU.draw(background);
        mainMenu.draw(MENU);

        MENU.display();
    }
}