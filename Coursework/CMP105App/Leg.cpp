#include "Leg.h"

Leg::Leg()
{
}

Leg::Leg(float width, float length, GameObject* torso, sf::Vector2f ATP, bool player1)
{
	(player1) ? thigh_tx.loadFromFile("gfx/Player/Leg.png", sf::IntRect(0,0,210,725)) : thigh_tx.loadFromFile("gfx/Player/Leg_Network.png", sf::IntRect(0, 0, 210, 725));
	(player1) ? calve_tx.loadFromFile("gfx/Player/Leg.png", sf::IntRect(0, 725, 210, 725)) : calve_tx.loadFromFile("gfx/Player/Leg_Network.png", sf::IntRect(0, 725, 210, 725));

	//each limb segment is a half the length of a leg and same width
	

	LEGScomponents[0] = new LimbSegment(torso, ATP);
	LEGScomponents[0]->setSize(sf::Vector2f(width, length/2));
	LEGScomponents[0]->setCollisionBox(0,0,width, length/2);
	LEGScomponents[0]->setOrigin(width/2,0);
	LEGScomponents[0]->setRotaryIndependence(true);
	LEGScomponents[0]->setRotationSpeed(kickSpeed);																		//kick rotation speed
	LEGScomponents[0]->setColliderType(ColliderType::THIGH);
	LEGScomponents[0]->setLimbTexture(&thigh_tx);

	LEGScomponents[1] = new LimbSegment(LEGScomponents[0], sf::Vector2f(width / 2, length /2));
	LEGScomponents[1]->setCollisionBox(0, 0, width, length/2);
	LEGScomponents[1]->setSize(sf::Vector2f(width, length/2));
	LEGScomponents[1]->setOrigin(width/2, 0);
	LEGScomponents[1]->setHinge(0, 120);
	LEGScomponents[1]->setColliderType(ColliderType::CALVE);
	LEGScomponents[1]->setLimbTexture(&calve_tx);
}

Leg::~Leg()
{
}

void Leg::draw(sf::RenderWindow* window)
{
	for (int i = 0; i < std::size(LEGScomponents); i++)
	{
		window->draw(*LEGScomponents[i]);
	}
}

void Leg::update(float dt, kick &kicking)
{
	//set everything according to direction faced

	for (int i = 0; i < std::size(LEGScomponents); i++)
	{
		LEGScomponents[i]->setFlipped(flipped);
	};

	//flip texture

	kickDirection = (flipped) ? -1 : 1;
	
	// kick or update ... kickdate
	switch (kicking)
	{
	case kick::NAE: 																						//no kick == run default
	default:
		for (int i = 0; i < std::size(LEGScomponents); i++)
		{
			LEGScomponents[i]->update(dt);

			LEGScomponents[1]->setRotation(0); // needs resetting 
			LEGScomponents[1]->setRotation(0);
		}
		break;

	case kick::LEG:
		LEGScomponents[1]->setLocked(true);
		LEGScomponents[0]->setRotationSpeed(kickSpeed);												// full leg kick is slower but can yied higher velocity
																											//if theres a kick rotate the leg to (max amount allowed) degrees then bring it back
	case kick::KNEE:																						//if its a knee kick rotate just thigh and keep th rest as its supposed to be
		if (!kicked)
		{
			LEGScomponents[0]->setRotating(static_cast<rotationDir>(-kickDirection));
			LEGScomponents[0]->update(dt);								
			kick_counter += LEGScomponents[0]->getRotationSpeed() * dt;
			if (kick_counter >= max_kick_angle) kicked = true;
		}
		else
		{
			LEGScomponents[1]->setLocked(false); //LEG

			LEGScomponents[0]->setRotating(static_cast<rotationDir>(kickDirection));
			LEGScomponents[0]->update(dt);
			kick_counter -= LEGScomponents[0]->getRotationSpeed() * dt;
			if (kick_counter <= 0)
			{
				kicked = false;
				LEGScomponents[0]->setRotating(rotationDir::NO_ROTATION);

				LEGScomponents[1]->setRotation(0); // needs resetting do to floats not going to 0
				LEGScomponents[1]->setRotation(0);

				LEGScomponents[0]->setRotationSpeed(kickSpeed*1.25f);
				
				kicking = kick::NAE;
			}
		}
		LEGScomponents[1]->update(dt);
		break;
		
	}
}
