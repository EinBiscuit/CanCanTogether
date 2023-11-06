#include "InputField.h"

InputField::InputField(sf::Vector2f size, sf::Vector2f position, std::string start_tag, sf::Font* font, int input_limit, bool only_integer) : Button(size, position, start_tag.append(input_limit,'W'), font)
{
	startTag = start_tag.erase(start_tag.size()-input_limit,input_limit);
	this->only_integer = only_integer;
	this->input_limit = input_limit;
	tag.setString(startTag + inString);
}

void InputField::handleInput(Input* input, char input_txt)
{
	if (!active &&Button::handleInput(input)) {
		active = true;
	}
	if (active)
	{
		body.setOutlineColor(sf::Color::Color(255, 140, 0, 255)); // set to orange
		if (!collisionbody.contains(sf::Vector2f(input->getMouseX(), input->getMouseY())) && input->isMouseLDown())
		{
			active = false;
		}
		
		if (inString == "INPUT HERE")
		{
			inString.clear();
		}

		if (input->isKeyDown(sf::Keyboard::BackSpace))
		{
			if (!inString.empty())
			{
				inString.pop_back();
			}
			input->setKeyUp(sf::Keyboard::BackSpace);
		}
		else
		if ((inString.size() < input_limit || !input_limit) && input_txt)
		{
			if (only_integer)
			{
				if (input_txt>47 && input_txt<58) inString += input_txt;
			}
			else
			{
				inString += input_txt;
			}
		}
		tag.setString(startTag + inString);
	}
	else
	{
		if (inString.empty())
		{
			inString = "INPUT HERE";
			tag.setString(startTag + inString);
		}
	}
}
