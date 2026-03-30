#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) : mainMenu {Text(font), Text(font), Text(font)}
{
	if (!font.openFromFile("Fonts/AlexandriaFLF.ttf"))
	{
		cout << "No font is here";
	}

	MainMenuSelected = 0;

	//Play
	mainMenu[0].setFont(font);
	mainMenu[0].setFillColor(Color::White);
	mainMenu[0].setString("Play");
	mainMenu[0].setCharacterSize(70);
	mainMenu[0].setPosition({ 400,200 });

	//Options
	mainMenu[1].setFont(font);
	mainMenu[1].setFillColor(Color::White);
	mainMenu[1].setString("Options");
	mainMenu[1].setCharacterSize(70);
	mainMenu[1].setPosition({ 400,300});

	//Exit
	mainMenu[2].setFont(font);
	mainMenu[2].setFillColor(Color::White);
	mainMenu[2].setString("Exit");
	mainMenu[2].setCharacterSize(70);
	mainMenu[2].setPosition({ 400,400});

	mainMenu[MainMenuSelected].setFillColor(Color::Yellow);

}
MainMenu::~MainMenu()
{

}

//Draw MainMenu

void MainMenu::draw(RenderWindow& window) {
	for (int i = 0; i < Max_main_menu; i++)
	{
		window.draw(mainMenu[i]);
	}
}

//Move up

void MainMenu::MoveUp() {
	if (MainMenuSelected - 1 >= 0) {
		mainMenu[MainMenuSelected].setFillColor(Color::White);

		MainMenuSelected = (MainMenuSelected - 1 + Max_main_menu) % Max_main_menu;

		mainMenu[MainMenuSelected].setFillColor(Color::Yellow);
	}
}

//Move down

void MainMenu::MoveDown() {
	if (MainMenuSelected + 1 < Max_main_menu) {
		mainMenu[MainMenuSelected].setFillColor(Color::White);

		MainMenuSelected = (MainMenuSelected + 1) % Max_main_menu;

		mainMenu[MainMenuSelected].setFillColor(Color::Yellow);
	}
}