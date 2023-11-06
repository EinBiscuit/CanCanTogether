#pragma once
#include "SFML\Graphics.hpp"
#include "Framework\Input.h"
#include "Framework\GameState.h"
#include "Button.h"
#include "Framework\AudioManager.h"

class Menus
{
protected:
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;
	AudioManager* audio;

	Menus() { window = nullptr; input = nullptr; gameState = nullptr; };
	~Menus() {};

	Menus(sf::RenderWindow* hwnd, Input* input, GameState* state, AudioManager* aud) { window = hwnd; this->input = input; gameState = state; audio = aud; };
	virtual void handleInput() {};
	virtual void draw() {};

	void beginDraw();
	void endDraw();

};

