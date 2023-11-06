#pragma once
#include "Framework/GameObject.h"
class Score_Manager :
	public GameObject
{
	sf::RenderWindow* window;
	sf::Font* font;
	sf::Text score;
	
	uint16_t SCORE = 0;

public:
	Score_Manager() {};
	Score_Manager(sf::RenderWindow* windae, sf::Font* tnof);
	~Score_Manager() {};
	void update();
	void draw();
	void increment(int by);

	void setScore(int score) { SCORE = score; }
	uint16_t getScore() { return SCORE; };
};

