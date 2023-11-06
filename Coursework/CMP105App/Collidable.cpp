#include "Collidable.h"

#define REDUCT_VELOCITY 0.9f

Collidable::Collidable()
{
}

Collidable::Collidable(int num)
{
	instanceNum = num;
}

Collidable::~Collidable()
{
}

void Collidable::collisionResponse(GameObject* collider, float dt) // for square objects 
{
	sf::Transform transform;
	float col_angle = 0; // rotation angle of the collider
	float rel_obj_angle = 0;//rotation of an object relative to the colliders rotation
	sf::Vector2f col_normal;
	sf::Vector2f rel_dir;
	float dot_x;
	float dot_y;
	int direcSign_X;
	int direcSign_Y;

	//before responding move away from the collider 

	move(Collision::relativePosition(collider, this) * dt);

	//rotation response is same and will only have slight changes depending on collider

	rel_obj_angle = std::fmod(getRotation()-collider->getRotation(),90.f); // rotation relative to the collider with a period of 45 because all collision boxes are squares
	
	//set rotation dir

	if (rel_obj_angle == 0 || rel_obj_angle == 45) setRotating(rotationDir::NO_ROTATION);
	else
	{

		if (rel_obj_angle > 45)
		{
			setRotating(rotationDir::CLOCKWISE);
			setRotationSpeed(rel_obj_angle - 45);
		}
		else
		{
			setRotating(rotationDir::C_CLOCKWISE);
			setRotationSpeed(45 - rel_obj_angle);
		}
	}

	switch (collider->getColliderType())
	{
	default:
		break;
		// send off the collidebale with direction of a hit side and speed of rotation + distance from leg root

	case ColliderType::CALVE :									
		
		col_normal = (collider->getTransform().transformPoint(collider->getPoint(0)) - collider->getTransform().transformPoint(collider->getPoint(1))) * ((collider->isFlipped()) ? 1.f : -1.f);

		if (Vector::dot(Collision::relativePosition(collider, this), col_normal) > 0) //check that leg is facing obects direction
		{
			velocity =
				// direction // vector from ceter of collider to object + colliders side normal
				Vector::normalise(Collision::relativePosition(collider, this) + col_normal)

				*
				// speed // distance from leg root (includes thigh length) + rotation speed
				(Vector::magnitude(Collision::relativePosition(this, dynamic_cast<LimbSegment*>(collider)->getMaster()->getPosition())) + collider->getRotationSpeed() * ((collider->getRotationDir() ? 1.f : 0.f)))

				*
				1.5f;
			
		}
		else velocity = -velocity*REDUCT_VELOCITY; // if collidable hits a leg not on the front side aka kick direction side
		break;


	case ColliderType::THIGH:

		col_normal = (collider->getTransform().transformPoint(collider->getPoint(0)) - collider->getTransform().transformPoint(collider->getPoint(1))) * ((collider->isFlipped()) ? 1.f : -1.f);
		
		if (Vector::dot(Collision::relativePosition(collider, this), col_normal) > 0) //check that leg is facing right dirrection
		{
			velocity =
				// direction // vector from ceter of collider to object + colliders side normal
				Vector::normalise(Collision::relativePosition(collider, this) + col_normal)

				*
				// speed // distance from leg root + rotation speed
				(Vector::magnitude(Collision::relativePosition(this, collider->getPosition())) + collider->getRotationSpeed() * ((collider->getRotationDir() ? 1.f : 0.f)))

				*
				1.5f;
		}
		else velocity = -velocity*REDUCT_VELOCITY; // if collidable hits a leg not on the front side aka kick direction side
		break;

	
	case ColliderType::HEAD :
	case ColliderType::STAGE:

		// if collider rotation is > 180 then treat it as negative  rotation up to 

		col_angle = collider->getRotation();
		transform.rotate(2.f*col_angle);

		

		// depending on relative position reflect accordingly // find the side and reflect in it
		
		rel_dir = Collision::relativePosition(collider, this);
		
		direcSign_X = (rel_dir.x > 0) ? 1 : -1;
		direcSign_Y = (rel_dir.y > 0) ? 1 : -1; 

		dot_x = Vector::dot(Collision::relativePosition(collider, this), sf::Vector2f(collider->getSize().x * 0.5f * direcSign_X, 0)) - 0.5f * collider->getSize().x;
		dot_y = Vector::dot(Collision::relativePosition(collider, this), sf::Vector2f(0, collider->getSize().y * 0.5f * direcSign_Y)) - 0.5f * collider->getSize().y;

		if (dot_x < dot_y)
		{
			velocity = { -velocity.x, velocity.y };
		}
		else if (dot_x > dot_y)
		{
			velocity = { velocity.x, -velocity.y };
		}
		else
		{
			velocity = -velocity;
		}
		
		velocity = velocity * REDUCT_VELOCITY;

		velocity = transform.transformPoint(velocity);

		if (collider->getColliderType() == ColliderType::HEAD)
		{
			velocity = -velocity;
			velocity += collider->getVelocity(); // account for player velocity
		}

		break;



	case ColliderType ::COLLIDABLE :
		
		// change their velocity to sum of one another
		sf::Vector2f NewVelocity;
		if (Vector::dot(velocity, collider->getVelocity()) < 0)
		{
			NewVelocity = transform.transformPoint((velocity + collider->getVelocity()));
			collider->setVelocity(NewVelocity);
			velocity = NewVelocity;
		}
		break;
	}
}

void Collidable::update(float dt)
{
	if(alive)
	{
		//if (freefall) setting for birbs which never happened
		velocity += gravity;

		move(velocity*dt);
		rotate(getRotationSpeed() * getRotationDir() * dt);
	}
}
