#include "Foreground.h"

Foreground::Foreground(Player* pl)
{
	player = pl;

	foreground_tx.loadFromFile("gfx/Backgrounds/Foreground.png");
	setTexture(&foreground_tx);
}

void Foreground::setup(sf::RenderWindow* window)
{
	setSize(sf::Vector2f(window->getSize().x+64, window->getSize().y+36));//set it to a slighly bigger size, keep 16 : 9
	setPosition(-32, -18);// offset by half
}

void Foreground::update(float dt)
{
	if (player->getPlayerState() == Player::PlayerState::WALK)
	{
		move(-(player->getVelocity()) * dt * 0.05f);
	}
}

void Foreground::reset()
{
	setPosition(-32, -18);// offset by half
}


