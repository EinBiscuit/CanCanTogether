#include "GameObject.h"

GameObject::GameObject()
{
	input = nullptr;
}

GameObject::~GameObject()
{

}

// Override this function to provide input handling for the derived class
void GameObject::handleInput(float dt)
{
}

// override this function to provide functionality
void GameObject::update(float dt)
{
}

// Sets the velocity of the sprite
void GameObject::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}
void GameObject::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

// get sprite velocity
sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}

// Returns collision box + position, giving the collision box in the correct position //EVEN IF ORIGIN IS CHANGED
sf::FloatRect GameObject::getCollisionBox() 
{	
	return sf::FloatRect(collisionBox.left + getPosition().x-getOrigin().x, collisionBox.top + getPosition().y-getOrigin().y, collisionBox.width, collisionBox.height); 
}

sf::Vector2f GameObject::getCollisionBoxLocalPoint(int n) // returns a local point of CB for OBB
{
		switch (n)
	{
		case 0: return sf::Vector2f(collisionBox.left, collisionBox.top);
		break;
	case 1: return sf::Vector2f(collisionBox.left + collisionBox.width, collisionBox.top);
		break;
	case 2: return sf::Vector2f(collisionBox.left + collisionBox.width, collisionBox.top + collisionBox.height);
		break;
	case 3: return sf::Vector2f(collisionBox.left, collisionBox.top + collisionBox.height);
		break;
	}
}

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void GameObject::collisionResponse(GameObject * collider, float dt)
{
}
