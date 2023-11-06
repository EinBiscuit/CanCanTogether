#pragma once
#include "Menus.h"
class Menu_Credits :
	public Menus
{	
	GameState* state;
	Button* Back;
	sf::Texture credits_tx;
	sf::Font font;
	sf::Sprite Credits;

public:

	Menu_Credits(sf::RenderWindow* windae, Input* inp, GameState* state);
	void handleInput();
	void draw();
	
};

