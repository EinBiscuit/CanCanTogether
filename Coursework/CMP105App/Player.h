#pragma once
#include "Framework/GameObject.h"
#include "LimbSegment.h"
#include "Framework/Collision.h"
#include "Leg.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"

class Player :  // aka butt aka LeG manager :D 
	public GameObject
{
	friend class Network;

protected:
	AudioManager* audio;

	LimbSegment* Head;
	LimbSegment* Butte;
	Leg::kick smack;
	Leg* LEGS[2];
	int kicker;
	bool kicked = false;
	LimbSegment* componentCollision(GameObject* collider, LimbSegment* bodypart);

	sf::Texture Head_tx;
	Animation Head_an_Idle;
	Animation Head_an_Hit;
	Animation* HeadAnimation = &Head_an_Idle;

	bool stagger = false;
	float Timer = 0;
	float Stag_tresh = 1.2f;

	float WalkSpeed = 1.2;

public:
	enum class PlayerState { IDLE, WALK };
	PlayerState playerState;

	//constructors
	Player(sf::Vector2f size, AudioManager* aud, bool player1 = true); //size for head sizePlayer hass general baby proporions 1/3 or 1/4 head to body;
	Player();
	~Player();

	//ovverides
	void update(float dt);
	void handleInput(float dt, Input* input);
	void draw(sf::RenderWindow* window);
	void collisionResponse(GameObject* collider, float dt);

	//etc
	void setPlayerState(PlayerState ps) { playerState = ps; };
	PlayerState getPlayerState() { return playerState; };
	
	void setHeadAnimation(bool b);
	Animation* getHeadAnimation();

	// for network

	int getKickerLeg() { return kicker; };
	Leg::kick getKickerState() { return smack; };
	bool getStagger() { return stagger; };

	void setKickerLeg(int kickl) { kicker = kickl; };
	void setKickerState(Leg::kick kick) { smack = kick; };
	void setStagger(bool stag) { stagger = stag; };
};

