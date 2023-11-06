#pragma once
#include "Menus.h"
#include "Score_Manager.h"
#include "Player.h"
#include "Framework/Animation.h"

class Menu_END :
	public Menus
{
	Button* Title;

	Button* button[3];
	Score_Manager* score;
	Player* player;
	
	sf::RectangleShape body;
	sf::Texture body_tx;
	Animation END;
	sf::Vector2f TargetPos;

	bool quack = false;

public:
	Menu_END();
	~Menu_END();

	Menu_END(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud, Score_Manager* scr, Player* plr, sf::Font* font);

	void update(float dt);
	void handleInput();
	void draw();
	void reset();
};

