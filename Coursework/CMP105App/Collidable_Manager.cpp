#include "Collidable_Manager.h"

Collidable_Manager::Collidable_Manager(sf::RenderWindow* windae, AudioManager* aud, Score_Manager* scr)
{
	window = windae;
	audio = aud;
	score = scr;

	bread_tx.loadFromFile("gfx/Bird/bread.png");
	bird_tx.loadFromFile("gfx/Bird/Bird.png");


	srand(time(NULL));
	//srand(1);
}

void Collidable_Manager::spawn()
{
	
	if (Timer >= Threshhold && CanSpawn) //enlarge object population
	{
		Timer = 0;
		DEAD.push_back(new Collidable(collidableCounter));
		DEAD.back()->setSize(sf::Vector2f(50, 50));
		DEAD.back()->setCollisionBox(sf::FloatRect(0, 0, 40, 40));
		DEAD.back()->setColliderType(ColliderType::COLLIDABLE);
		DEAD.back()->setOrigin(0.5f * DEAD.back()->getSize());
		DEAD.back()->setRotationSpeed(180);
		DEAD.back()->setRotation(25);
		DEAD.back()->setTexture(&bread_tx);
		collidableCounter++;
	}
	
	if (!DEAD.empty())
	{
		//R E I N C A R N A T E
		DEAD.back()->setAlive(true);
	
		//S E T P O S I T I O N/ spawn
		DEAD.back()->setPosition(sf::Vector2f(10 + rand() % (window->getSize().x - 20), 0));
		DEAD.back()->setVelocity(0, 0);
	
		// REMOVE FROM THE DEAD
		ALIVE.push_back(DEAD.back());
		DEAD.pop_back();
	}
	
}

void Collidable_Manager::update(float dt , std::vector<GameObject*> entities)
{
	Timer += dt; // increment timer

	for (auto spawnable = ALIVE.begin(); spawnable != ALIVE.end();) 
	{
		if (!(*spawnable)->isAlive()) //if dead erase it from alive
		{
			DEAD.push_back(*spawnable);
			spawnable = ALIVE.erase(spawnable);
		}
		else						// if alive 
		{
			(*spawnable)->update(dt);

			if ((*spawnable)->getPosition().x < 0 || (*spawnable)->getPosition().x > window->getSize().x) // if is outside  boundaries
			{
				(*spawnable)->setAlive(false);	// make dead
				score->increment(10);			// increment score;
			}
			else
			{
				for (auto object : entities) // check for collisions with environment
				{
					if (Collision::checkOBBvsOBB((*spawnable), object))
					{
						if (object->getColliderType() == ColliderType::PLAYER) // player has to run internal check of limbs for
						{
							object->collisionResponse(*spawnable, dt);
						}
						else
						{
							(*spawnable)->collisionResponse(object, dt);
						}
					}
				}
				for (auto object : ALIVE)
				{
					if (object != *spawnable && Collision::checkOBBvsOBB(*spawnable, object))
					{
						(*spawnable)->collisionResponse(object,dt);
					}
				}
			}
			spawnable++;
		}
	}
}

void Collidable_Manager::draw()
{
	for (auto i : ALIVE)
	{
		window->draw(*i);
	}
}

void Collidable_Manager::reset()
{
	Timer = 0;
	score->setScore(0);
	for (auto i : ALIVE)
	{
		delete i;
		i = NULL;

	}
	for (auto i : DEAD)
	{
		delete i;
		i = NULL;
	}
	ALIVE.clear();
	DEAD.clear();
}


