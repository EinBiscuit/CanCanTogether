#include "Menu_Main.h"

Menu_Main::Menu_Main()
{
}

Menu_Main::~Menu_Main()
{
}

Menu_Main::Menu_Main(sf::RenderWindow* windae, Input* input, GameState* state, AudioManager* aud) : Menus(windae, input, state, aud)
{
	//properties
	font.loadFromFile("font/CanCandB_free.ttf");

	sf::Vector2f size = window->getView().getSize() * 0.1f;
	sf::Vector2f position = window->getView().getSize() * 0.1f;
	sf::Vector2f spacingY = { 0, window->getView().getSize().y * 0.3f };

	//title

	Title.setFillColor(sf::Color::Black);
	Title.setFont(font);
	Title.setString("cCAN CANd");
	Title.setCharacterSize(size.x);
	Title.setOrigin(0, Title.getGlobalBounds().height * 0.5f);
	Title.setPosition(position.x*4.2f,position.y*1.5f);

	//buttons

	button[0] = new Button(size*1.5f, position, "> PLAY", &font);
	position += spacingY;
	button[1] = new Button(size, position, "> CREDITS & INSTRUCTIONS", &font);
	position += spacingY;
	button[2] = new Button(size, position, "> EXIT", &font);
	
	
}

void Menu_Main::handleInput()
{
	//play game
	if (button[0]->handleInput(input))
	{
		gameState->setCurrentState(State::LEVEL);
		audio->stopAllMusic();
	}
	//credits;
	if(button[1]->handleInput(input)) gameState->setCurrentState(State::CREDITS);
	//Exit
	if(button[2]->handleInput(input)) window->close();
	
}

void Menu_Main::render()
{
	//beginDraw();

	window->draw(Title);
	for (int i = 0; i < 3; i++)
	{
		button[i]->draw(window);
	}

	//endDraw();
}
