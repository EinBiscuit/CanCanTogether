#include "Background.h"

Background::Background(Player* pl)
{
	player = pl;

	base_tx.loadFromFile("gfx/Backgrounds/Backgrounds_1.png");
	stage_tx.loadFromFile("gfx/Backgrounds/Backgrounds_2.png");
	curtain_tx.loadFromFile("gfx/Backgrounds/Backgrounds_3.png");
}

void Background::setup(sf::RenderWindow* window)
{
	for (int i = 0; i < 3; i++)
	{
		bg[i] = new sf::RectangleShape;
		bg[i]->setSize(sf::Vector2f(window->getSize().x + 96, window->getSize().y + 54));//set it to a slighly bigger size, keep 16 : 9 96 54
		bg[i]->setPosition(-48,-27);// offset by that size;
	}
	bg[0]->setTexture(&base_tx);
	bg[1]->setTexture(&stage_tx);
	bg[2]->setTexture(&curtain_tx);

}

void Background::update(float dt)
{
	if (player->getPlayerState() == Player::PlayerState::WALK)
	{
		for (int i = 0; i < 3; i++)
		{
			bg[i]->move(-(player->getVelocity()) * dt * 0.05f * (1.f+i));
		}
	}
}

void Background::draw(sf::RenderWindow* window)
{
	for (int i = 0; i < 3; i++)
	{
		window->draw(*bg[i]);
	}
}

void Background::reset()
{
	for (int i = 0; i < 3; i++)
	{	 
		bg[i]->setPosition(-48, -27);// offset by that size;	
	}
}


