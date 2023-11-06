#pragma once
#include "SFML\Graphics.hpp"
#include "Framework\Input.h"

class Button
{
protected:
	sf::RectangleShape body;
	sf::FloatRect collisionbody;
	sf::Text tag;
	
public:
	Button(sf::Vector2f size, sf::Vector2f position, std::string content, sf::Font* font);
	bool handleInput(Input* input);
	void draw(sf::RenderWindow* window);

};

