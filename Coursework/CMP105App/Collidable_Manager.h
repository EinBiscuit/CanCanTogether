#pragma once
#include "Collidable.h"
#include "list"
#include "ctime"
#include "Score_Manager.h"

class Collidable_Manager
{
	friend class Network;

protected:
	AudioManager* audio;
	sf::RenderWindow* window;
	Score_Manager* score;

	sf::Texture bread_tx;
	sf::Texture bird_tx;

	std::vector<Collidable*> DEAD;
	std::list<Collidable*> ALIVE; // or are they

	float Timer = 0;
	float Threshhold = 10.f;

	int collidableCounter = 0;

public:
	Collidable_Manager();
	Collidable_Manager(sf::RenderWindow* windae, AudioManager* aud, Score_Manager* scr);

	std::list<Collidable*>* getAlive() { return &ALIVE; }
	void spawn();
	void update(float dt , std::vector<GameObject*> entities);
	void draw();
	void reset();

	bool CanSpawn = true;
};

