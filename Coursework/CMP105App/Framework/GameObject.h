// Game Object Class
// A class representing a single renderable game object.
// Game Object extend sf::RectangleShape, and added functionailty for collision, sprite states (alive/dead), velocity for movement, input component
// Most elements of your game will extend game object to create larger more complex elements such as player, enemies, bullets etc.
// @author paul Robertson

#pragma once
#include "SFML\Graphics.hpp"
#include "Input.h"

enum  rotationDir { C_CLOCKWISE = -1, NO_ROTATION = 0, CLOCKWISE = 1 }; //E X P A N D
enum class ColliderType { STAGE, PLAYER, HEAD, CALVE, THIGH, COLLIDABLE };

class GameObject : public sf::RectangleShape
{
public:
	
	GameObject();
	~GameObject();

	// Virtual update function. Base function does nothing (for static objects). Inheritted version will most likely override this function.
	virtual void handleInput(float dt);
	virtual void update(float dt);

	// Control sprite speed and direction
	void setVelocity(sf::Vector2f vel);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity();
	
	//R O T A T I O N S
	void setRotationSpeed(float f) { RotationSpeed = f; };
	float getRotationSpeed() { return RotationSpeed; };
	void setRotating(rotationDir d) { Rotating = d; };
	rotationDir getRotationDir() { return Rotating; };


	// Sprite state
	bool isAlive() { return alive; };
	void setAlive(bool b) { alive = b; };

	void setFlipped(bool F) { flipped = F; };
	bool isFlipped() { return flipped; };

	ColliderType getColliderType() { return ColliderTag; }
	void setColliderType(ColliderType type) { ColliderTag = type; };

	// For sprite collision, set collider box, get collider box, and dedicated virtual function for collision response
	bool isCollider() { return collider; };
	void setCollider(bool b) { collider = b; };
	sf::FloatRect getCollisionBox(); // editted :3
	sf::Vector2f getCollisionBoxLocalPoint(int n); // insertion for OBB collisions
	void setCollisionBox(float x, float y, float width, float height) { collisionBox = sf::FloatRect(x, y, width, height); };
	void setCollisionBox(sf::FloatRect fr) { collisionBox = fr; };
	virtual void collisionResponse(GameObject* collider, float dt);

	// Set the input component
	void setInput(Input* in) { input = in; };


protected:
	// Sprite properties
	sf::Vector2f velocity;

	 // E X P A N D
	float RotationSpeed;
	rotationDir Rotating;
	
	ColliderType ColliderTag;
	bool flipped = false;
	bool alive;

	// Collision vars
	sf::FloatRect collisionBox;
	bool collider;

	// input component
	Input* input;



	
};