#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
using namespace std;
using namespace sf;

#define Max_main_menu 3
class MainMenu
{
public:
	MainMenu(float width, float height);

	void draw(RenderWindow& window);
	void MoveUp();
	void MoveDown();

	int MainMenuPressed() {
		return MainMenuSelected;
	}
	~MainMenu();

private:
	int MainMenuSelected;
	Font font;
	array<Text, Max_main_menu> mainMenu;

};

