#include "Menu_Credits.h"

Menu_Credits::Menu_Credits(sf::RenderWindow* windae, Input* inp, GameState* stt)
{
	state = stt;
	input = inp;
	window = windae;

	credits_tx.loadFromFile("gfx/Credits.png");
	font.loadFromFile("font/CanCandB_free.ttf");

	Credits.setTexture(credits_tx);
	Credits.setScale(0.92,0.92);
	Credits.setPosition(-10, 0);

	sf::Vector2f size = sf::Vector2f( window->getSize()) * 0.2f;
	sf::Vector2f position = sf::Vector2f( window->getSize().x*0.49f, window->getSize().y*0.75f);

	Back = new Button(size, position,"> BACK <", &font);
}

void Menu_Credits::handleInput()
{
	if (Back->handleInput(input) || input->isKeyDown(sf::Keyboard::Escape))
	{
		input->setKeyUp(sf::Keyboard::Escape);
		state->setCurrentState(state->getLastState());
	}
}

void Menu_Credits::draw()
{
	//beginDraw();
	window->draw(Credits);
	Back->draw(window);
	//endDraw();
}
