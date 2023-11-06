#pragma once
#include "Framework/GameObject.h"
#include "LimbSegment.h"
class Leg :
	public GameObject
{
	LimbSegment* LEGScomponents[2];
	sf::Texture thigh_tx;
	sf::Texture calve_tx;
	
	float max_kick_angle = 110;
	float kickSpeed = 465;
	float kick_counter = 0;
	bool kicked = false;
	int kickDirection = 1;

	
	
public:
	//speshal things
	enum class kick { NAE, KNEE, LEG };
	
	kick dflt = kick::NAE;

	//constructors

	Leg();
	Leg(float length, float width, GameObject* torso, sf::Vector2f ATP, bool player1 = true);
	~Leg();

	//overides

	void draw(sf::RenderWindow* window);
	void update(float dt, kick &type);
	
	//etc
	LimbSegment* getComponent(int n) { return LEGScomponents[n]; }
};

