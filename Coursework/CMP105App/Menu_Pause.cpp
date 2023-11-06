#include "Menu_Pause.h"

Menu_Pause::Menu_Pause()
{
}

Menu_Pause::~Menu_Pause()
{
}

Menu_Pause::Menu_Pause(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud) : Menus(windae, input, state, aud)
{
	//properties
	font.loadFromFile("font/CanCandB_free.ttf");

	sf::Vector2f size = window->getView().getSize()*0.5f;
	sf::Vector2f position = window->getView().getSize() * 0.1f;
	sf::Vector2f spacingY = { 0, window->getView().getSize().y * 0.3f };

	Box.setSize(size);
	Box.setOrigin(size * 0.5f);
	Box.setPosition(position * 5.f);
	Box.setOutlineThickness(size.x * 0.1f);
	Box.setOutlineColor(sf::Color::Black);

	//title

	Title.setFillColor(sf::Color::Black);
	Title.setFont(font);
	Title.setString("> PAUSE <");
	Title.setCharacterSize(size.x*0.2f);
	Title.setOrigin(Title.getGlobalBounds().width * 0.5f, Title.getGlobalBounds().height * 0.5f);
	Title.setPosition(Box.getPosition().x,Box.getPosition().y-position.y);


	//buttons

	button[0] = new Button(size * 0.1f, sf::Vector2f(position.x * 2.8f, position.y * 6.f), "RESUME", &font);
	button[1] = new Button(size * 0.1f, sf::Vector2f(position.x * 3.85f, position.y * 6.f), "RESTART", &font);
	button[2] = new Button(size * 0.1f, sf::Vector2f(position.x * 4.95f, position.y * 6.f), "MENU", &font);
	button[3] = new Button(size * 0.1f, sf::Vector2f(position.x * 5.83f, position.y * 6.f), "INSTRUCTIONS", &font);
	
}

void Menu_Pause::handleInput()
{
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		input->setKeyUp(sf::Keyboard::Escape);
		gameState->setCurrentState(State::LEVEL);
	}
	//play game
	if (button[0]->handleInput(input))
	{
		gameState->setCurrentState(State::LEVEL);
	}
	//credits;
	if (button[1]->handleInput(input)) gameState->setCurrentState(State::RESTART);
	//Exit
	if (button[2]->handleInput(input))
	{
		gameState->setCurrentState(State::MENU);
		audio->stopAllMusic();
	}
	//Instructions
	if (button[3]->handleInput(input)) gameState->setCurrentState(State::CREDITS);
}

void Menu_Pause::render()
{
	//beginDraw();

	window->draw(Box);
	window->draw(Title);

	for (int i = 0; i < 4; i++)
	{
		button[i]->draw(window);
	}

	//endDraw();
}