#include "Score_Manager.h"

Score_Manager::Score_Manager(sf::RenderWindow* windae, sf::Font* tnof)
{
	window = windae; font = tnof;
	
	score.setFont(*font);
	score.setCharacterSize(window->getSize().y*0.05f);
	score.setString("c SCORE:   d");
	sf::FloatRect textRect = score.getLocalBounds();

	//body size and position
	setPosition(window->getSize().x * 0.5f, window->getSize().y * 0.95f);
	setFillColor(sf::Color::Black);
	setSize(sf::Vector2f(textRect.width + window->getSize().y * 0.05f, window->getSize().y * 0.05f));
	setOrigin(getSize() * 0.5f);
	setOutlineThickness(window->getSize().y * 0.01f);
	setOutlineColor(sf::Color::White);

	score.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	score.setPosition(getPosition());

}
void Score_Manager::update()
{
	std::string scorestring = "c SCORE: " + std::to_string(SCORE) + " d";
	score.setString(scorestring);
	sf::FloatRect textRect = score.getLocalBounds();

	//body size and position
	setSize(sf::Vector2f(textRect.width + window->getSize().y * 0.05f, window->getSize().y * 0.05f));
	setOrigin(getSize() * 0.5f);

	score.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	score.setPosition(getPosition());

	score.setFillColor(sf::Color::White);
}

void Score_Manager::draw()
{
	window->draw(*this);
	window->draw(score);
}
void Score_Manager::increment(int by)
{
	SCORE += by;
	score.setFillColor(sf::Color::Color(255, 140, 0, 255));
}

