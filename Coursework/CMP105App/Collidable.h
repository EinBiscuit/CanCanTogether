#pragma once
#include "Framework/GameObject.h"
#include "Framework/Vector.h"
#include "Framework/Collision.h"
#include "Framework/AudioManager.h"
#include "Player.h"
#include "math.h"


class Collidable :
	public GameObject
{
	AudioManager* audio;
	sf::Vector2f gravity = { 0 , 0.098f * 5}; // gravity - its a  weird number but it feels nice ;3
	bool freefall = true;
	int instanceNum = -1;

public:

	// constructors
	Collidable();
	Collidable(int num);
	Collidable(AudioManager* aud) { audio = aud; };
	~Collidable();

	// overrides
	void collisionResponse(GameObject* collider, float dt);
	void update(float dt);
	//etc.
	void setFreefall(bool b) { freefall = b;};

	int getInstanceNum() { return instanceNum; };
};

