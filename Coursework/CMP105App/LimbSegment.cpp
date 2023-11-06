#include "LimbSegment.h"

LimbSegment::LimbSegment()
{

}

LimbSegment::~LimbSegment()
{

}

LimbSegment::LimbSegment(GameObject* master)
{
	this->master = master;
}

LimbSegment::LimbSegment(GameObject* master, sf::Vector2f m_local)
{
	this->master = master;
	AttachmentPoint = m_local;
}

void LimbSegment::setFlipped(bool F)
{
	if (flipped != F) {

		flipped = F;

		float temp = lowerHinge;
		lowerHinge = upperHinge;
		upperHinge = temp;

		if (flipped)setTextureRect(Texture_Flip); // flip texture
		else setTextureRect(Texture_dflt);
	}
}

void LimbSegment::rotate(float angle)
{
	if (angle)
	{
		if (hinged) {

			setRotation(getRotation() + counter); // puts rotation into non offset state

			if (!locked)
			{
				counter += angle;
				counter = std::max(-lowerHinge, std::min(counter, upperHinge));
			}
			setRotation(getRotation() + angle - counter);
		}
		else
		{
			setRotation(getRotation() + angle);
		}
	}
	//setRotation(counter);// opposite of what is above, also, peculiar
}
  
void LimbSegment::update(float dt)
{
	//set up

	setFlipped(master->isFlipped());
	setVelocity(master->getVelocity());
	setPosition(master->getTransform().transformPoint(AttachmentPoint)); // moves with master

	
	//rotation

	if (!rot_independ) //update rotation of the object if its not rotatry independent from its master calves whose movement entirely depends on thighs or butt;
	{
		setRotationSpeed(master->getRotationSpeed()); //should turn with the same speed as master, hinge has no friction (but could i guess)
		setRotating(master->getRotationDir());
	}

	rotate(getRotationSpeed() * dt * getRotationDir());
}
