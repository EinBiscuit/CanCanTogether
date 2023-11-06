#include "Player.h"
#define WALKMULT 4

LimbSegment* Player::componentCollision(GameObject* collider, LimbSegment* bodypart)
{
	{
		if (Collision::checkOBBvsOBB(collider, bodypart))
		{
			return bodypart;
		}

		if (bodypart->getMaster() == this) return NULL;

		componentCollision(collider, dynamic_cast <LimbSegment*> (bodypart->getMaster()));
	}
}

//public functions

Player::Player(sf::Vector2f HEADsize, AudioManager*aud, bool player1)
{
	audio = aud;

	setOrigin(HEADsize.x*4.f, HEADsize.y * 2);
	setCollisionBox(0.f, 0.f, HEADsize.x * 8, HEADsize.y * 4); // General purpouse collision box
	setColliderType(ColliderType::PLAYER);

	//setSize(sf::Vector2f(HEADsize.x * 8, HEADsize.y * 4)); //collision box visualiser 3000
	//setFillColor(sf::Color::Red);

	//BODY

	//head
	Head = new LimbSegment(this, sf::Vector2f(getOrigin().x, getOrigin().y - (2.f / 3.f * getOrigin().y)));
	Head->setSize(HEADsize*1.2f);
	Head->setOrigin(HEADsize*0.6f);
	Head->setCollisionBox(0.f, 0.f, HEADsize.x, HEADsize.y);
	Head->setColliderType(ColliderType::HEAD);

	//------------------------------------- // textures/animations
	(player1)? Head_tx.loadFromFile("gfx/Player/Head_sheet.png") : Head_tx.loadFromFile("gfx/Player/Head_sheet_Network.png");
	Head->setTexture(&Head_tx);

	// head animations
		//idle (1 frame)
	Head_an_Idle.addFrame(sf::IntRect(0,200,200,200));
		//on hit
	for (int i = 0; i < 3; i++)
	{
		Head_an_Hit.addFrame(sf::IntRect(200 * i, 0, 200, 200));
	}
	Head_an_Hit.setFrameSpeed(0.10f);
	Head_an_Hit.setLooping(false);

	
	//butte //pivot point for legs
	Butte = new LimbSegment(Head, sf::Vector2f(Head->getOrigin().x, Head->getOrigin().y + (1.f / 3.f * HEADsize.y)));
	Butte->setRotaryIndependence(true);
	Butte->setRotationSpeed(600);

	//legs
	LEGS[0] = new Leg(HEADsize.x / 2, HEADsize.y * 3.5f, Butte, sf::Vector2f( 1.f / 4.f * HEADsize.x, 0.f),player1);
	LEGS[1] = new Leg(HEADsize.x / 2, HEADsize.y * 3.5f, Butte, sf::Vector2f(-1.f / 4.f * HEADsize.x, 0.f),player1);

	kicker = 0;

	smack = Leg::kick::NAE;

	setRotationSpeed(180);
}

Player::~Player()
{

}

void Player::update(float dt)
{

	//set flipped based on mooving direction
	if (velocity.x > 0)setFlipped(false);
	else setFlipped(true);
	//
	HeadAnimation->setFlipped(!flipped); // HEAD SPRITE SHEET DRAWN THE OTHER WAY AROND
	//
	for (int i = 0; i < std::size(LEGS); i++)
	{
		LEGS[i]->setFlipped(flipped);
	}

	//move

	switch(playerState)
	{
		case PlayerState::IDLE:

			if ((Butte->getRotation() < 181 && Butte->getRotation() > 180) || (Butte->getRotation() < 1 && Butte->getRotation() > 0))
			{
				Butte->setRotating(rotationDir::NO_ROTATION);
			}

			break;
		case PlayerState::WALK:
			if (!flipped) 
			{
				Butte->setRotating(rotationDir::CLOCKWISE);
			}
			else 
			{
				Butte->setRotating(rotationDir::C_CLOCKWISE);
			}

			move(velocity*dt);

			break;
	}

	//component updates
	Head->update(dt);
	if (!HeadAnimation->getPlaying())
	{
			HeadAnimation->reset();
			HeadAnimation->setPlaying(true);
			HeadAnimation = &Head_an_Idle;
	}
	HeadAnimation->animate(dt);
	Head->setTextureRect(HeadAnimation->getCurrentFrame());


	Butte->update(dt);

	if (kicked && smack == Leg::kick::NAE)
	{
		kicker = (kicker) ? 0 : 1; //if zero set to one if one set to zero // switch the leg
		kicked = false;
	}

	LEGS[kicker]->update(dt, smack); //kicking leg
	LEGS[(kicker) ? 0 : 1]->update(dt, LEGS[(kicker) ? 0 : 1]->dflt); //run not kicker
	
}

void Player::handleInput(float dt, Input* input)
{
	if (stagger) // STAGGER THE INPUT
	{
		Timer += dt;
		if (Timer > Stag_tresh)
		{
			stagger = false;
			Timer = 0;
		}
	}

	if (input->isMouseLDown())
	{
		smack = Leg::kick::LEG;
		kicked = true;
	}
	if (input->isMouseRDown())
	{
		smack = Leg::kick::KNEE;
		kicked = true;
	}

	sf::Vector2f direction =  sf::Vector2f(input->getMouseX(),input->getMouseY()) - getPosition();
	
	if (smack == Leg::kick::NAE)
	{
		setVelocity(direction.x * ((stagger) ? WalkSpeed/2 : WalkSpeed), 0);			// walk speed // half if staggered
		Butte->setRotationSpeed(abs(direction.x)* WALKMULT * ((stagger) ? 0.5f : 1.2f)); // walk leg cartoon speed

		if (abs(direction.x) > Head->getSize().x*2.f)
		{
			setPlayerState(PlayerState::WALK);
		}
		else
		{
			setPlayerState(PlayerState::IDLE); 
		}
	}
	else
	{
		setPlayerState(PlayerState::IDLE);
	}
}

void Player::draw(sf::RenderWindow* window)
{
	window->draw(*this);
	LEGS[(kicker)?0:1]->draw(window); // not kicker
	LEGS[kicker]->draw(window);
	window->draw(*Head);
}

void Player::collisionResponse(GameObject* collider, float dt)
{
	if (LimbSegment* bodyPart = componentCollision(collider, LEGS[kicker]->getComponent(1)))
	{
		collider->collisionResponse(bodyPart, dt);
		if (bodyPart->getColliderType() == ColliderType::HEAD)
		{
			if (Vector::magnitude(collider->getVelocity()) > 60.f) // if head hit bad
			{
				HeadAnimation = &Head_an_Hit;
				audio->playSoundbyName("P_UGH");
				stagger = true;
			}
		}
	}
	
}

void Player::setHeadAnimation(bool b)
{
	if (b)
	{
		HeadAnimation = &Head_an_Hit;
	}
	else
	{
		HeadAnimation = &Head_an_Idle;
	}
}

Animation* Player::getHeadAnimation()
{
	return HeadAnimation;
}
