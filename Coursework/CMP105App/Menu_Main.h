#pragma once
#include "Menus.h"


class Menu_Main :
	public Menus
{
	sf::Font font;
	sf::Text Title;
	Button* button[3];

public :
	Menu_Main();
	~Menu_Main();

	Menu_Main(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud);

	sf::Font* GetFont() { return &font; };
	void handleInput();
	void render();
	
};

