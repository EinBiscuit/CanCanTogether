#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "Background.h"
#include "Foreground.h"
#include "LimbSegment.h"
#include "Leg.h"
#include "Player.h"
#include "Collidable.h"
#include "Collidable_Manager.h"
#include "Score_Manager.h"
#include "Menu_END.h"
#include <string>
#include <iostream>


class Level{
	friend class Network;

public:
	Level(sf::RenderWindow* hwnd, Input* in, Input* netIn, GameState* state, AudioManager* aud);
	~Level();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void restart();

	//CMP 302 extensions

	void GetDynamicObjects(Player*& player, Player*& pawn, Collidable_Manager* &Collidables);

protected:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	Input* ninput;
	GameState* gameState;
	AudioManager* audio;

	float Timer; // music countdown

	//Font
	sf::Font font;

	//Stage
	Background* background;
	Foreground* foreground;
	Score_Manager* score;

	GameObject Bumper_Right;
	GameObject Bumper_Left;
	GameObject Floor;

	Player* player;
	Player* pawn;

	Collidable_Manager* Collidables;

	Menu_END* THE_END;

	//TEST SHAPE
	GameObject Square1;

	Collidable Block;

	std::vector<GameObject*> space;
};