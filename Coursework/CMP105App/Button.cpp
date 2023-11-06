#include "Button.h"

Button::Button(sf::Vector2f size, sf::Vector2f position, std::string content, sf::Font* font)
{
	//text size
	tag.setFont(*font);
	tag.setCharacterSize(size.y * 0.5f);
	tag.setString(content);
	sf::FloatRect textRect = tag.getLocalBounds();

	//body size and position
	body.setPosition(position);
	body.setFillColor(sf::Color::Black);
	body.setSize(sf::Vector2f(textRect.width+size.x,size.y));
	body.setOutlineThickness(size.y*0.1f);

	collisionbody = sf::FloatRect(position.x, position.y, body.getSize().x, body.getSize().y);
	
	//text position
	tag.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	tag.setPosition(position+body.getSize()*0.5f);
}

bool Button::handleInput(Input* input)
{
	if (collisionbody.contains(sf::Vector2f(input->getMouseX(), input->getMouseY())))
	{
		body.setOutlineColor(sf::Color::Color(255, 140, 0, 255)); // set to orange
		tag.setStyle(tag.Underlined);
		if (input->isMouseLDown())
		{
			input->setMouseLDown(false);
			return true;
		}
	}
	else
	{
		tag.setStyle(tag.Regular);
		body.setOutlineColor(sf::Color::Transparent);
	}

	return false;
}

void Button::draw(sf::RenderWindow *window)
{
	window->draw(body);
	window->draw(tag);
}
