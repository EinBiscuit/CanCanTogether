#pragma once
#include "Framework/GameObject.h"
#include "Player.h"
#include "stdlib.h"

class Background :
	public GameObject
{
	sf::RectangleShape* bg[3];
	Player* player;

	sf::Texture base_tx;
	sf::Texture stage_tx;
	sf::Texture curtain_tx;

public:
	Background() {};
	Background(Player* pl);

	void setup(sf::RenderWindow *window);
	void update(float dt);
	void draw(sf::RenderWindow* window);
	void reset();
};

