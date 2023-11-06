#pragma once
#include "Menus.h"
class Menu_Pause :
	public Menus
{
	sf::Font font;
	sf::Text Title;
	sf::RectangleShape Box;
	sf::Texture* screenshot;
	Button* button[4];
	
public:
	Menu_Pause();
	~Menu_Pause();

	Menu_Pause(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud);

	void setScreenshot(sf::Texture* Prntscr) { Prntscr = screenshot; }
	void handleInput();
	void render();
};

