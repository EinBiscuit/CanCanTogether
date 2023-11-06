#pragma once
#include "Framework/GameObject.h"
#include "Player.h"
#include "stdlib.h"

class Foreground :
	public GameObject
{	
		Player* player;
		sf::Texture foreground_tx;

	public:
		Foreground() {};
		Foreground(Player* pl);

		void setup(sf::RenderWindow* window);
		void update(float dt);
		void draw(sf::RenderWindow* window);
		void reset();
};

