 #include "Collision.h"

// Check AABB for collision. Returns true if collision occurs.
bool Collision::checkBoundingBox(GameObject* s1, GameObject* s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2->getCollisionBox().left)
		return false;
	if (s1->getCollisionBox().left > s2->getCollisionBox().left + s2->getCollisionBox().width)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2->getCollisionBox().top)
		return false;
	if (s1->getCollisionBox().top > s2->getCollisionBox().top + s2->getCollisionBox().height)
		return false;

	return true;
}

// Check for bounding box collision with a point/vector2 rather than two boxes.
bool Collision::checkBoundingBox(GameObject* s1, sf::Vector2i s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2.x)
		return false;
	if (s1->getCollisionBox().left > s2.x)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2.y)
		return false;
	if (s1->getCollisionBox().top > s2.y)
		return false;

	return true;
}

// Check bounding circle collision. Returns true if collision occurs.
bool Collision::checkBoundingSphere(GameObject* s1, GameObject* s2)
{
	// Get radius and centre of sprites.
	float radius1 = s1->getSize().x / 2;
	float radius2 = s2->getSize().x / 2;
	float xpos1 = s1->getPosition().x + radius1;
	float xpos2 = s2->getPosition().x + radius2;
	float ypos1 = s1->getPosition().y + radius1;
	float ypos2 = s2->getPosition().y + radius2;

	if (pow(xpos2 - xpos1, 2) + pow(ypos2 - ypos1, 2) < pow(radius1 + radius2, 2))
	{
		return true;
	}
	return false;
}

//E N H A N C E

bool Overlap(float min1, float max1, float min2, float max2)
{
	if (min1 < min2) {

		if (min1 + max1 - min1 < min2)
			return false;
	}
	else
	{
		if (min2 + max2 - min2 < min1)
			return false;
	}

	return true;
}

void ProjectP(sf::Vector2f line, GameObject* sp, float& min, float& max)
{
	for (int i = 0; i < 4; i++)
	{
		float value = Vector::dot(line, sp->getTransform().transformPoint(sp->getCollisionBoxLocalPoint(i)));
		if (i == 0) { min = value; max = value; };
		if (value < min) min = value;
		if (value > max) max = value;
	}
}


bool Collision::checkOBBvsOBB(GameObject* sp1, GameObject* sp2)
{
	//store the projected volume data
	float vol1_min= 0;
	float vol1_max= 0;

	float vol2_min= 0;
	float vol2_max= 0;

	//store normal	
	sf::Vector2f normal;

	//sp1 get the normal of (0,0) (0,1);
	normal = { sp1->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(1)) - sp1->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(0)) };

	//project points of sp1 onto normal
	ProjectP(normal, sp1, vol1_min, vol1_max);

	//project points of sp2 onto normal
	ProjectP(normal, sp2, vol2_min, vol2_max);

	//compare volumes     volume size = vol1_max-vol1_min    // vol2_max-vol2_min
	if (!Overlap(vol1_min, vol1_max, vol2_min, vol2_max)) return false;
	  

	//sp1 get the normal of (0,0) (1,0);
	normal = { sp1->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(3)) - sp1->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(0)) };

	//project points of sp1 onto normal
	ProjectP(normal, sp1, vol1_min, vol1_max);

	//project points of sp2 onto normal
	ProjectP(normal, sp2, vol2_min, vol2_max);

	//compare volumes     volume size = vol1_max-vol1_min    // vol2_max-vol2_min
	if (!Overlap(vol1_min, vol1_max, vol2_min, vol2_max)) return false;
	
	

	//sp2 get the normal of (0,0) (0,1);
	normal = { sp2->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(1)) - sp2->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(0)) };

	//project points of sp1 onto normal
	ProjectP(normal, sp1, vol1_min, vol1_max);

	//project points of sp2 onto normal
	ProjectP(normal, sp2, vol2_min, vol2_max);

	//compare volumes     volume size = vol1_max-vol1_min    // vol2_max-vol2_min
	if (!Overlap(vol1_min, vol1_max, vol2_min, vol2_max)) return false;


	//sp2 get the normal of (0,0) (1,0);
	normal = { sp2->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(3)) - sp2->getTransform().transformPoint(sp1->getCollisionBoxLocalPoint(0)) };

	//project points of sp1 onto normal
	ProjectP(normal, sp1, vol1_min, vol1_max);

	//project points of sp2 onto normal
	ProjectP(normal, sp2, vol2_min, vol2_max);

	//compare volumes     volume size = vol1_max-vol1_min    // vol2_max-vol2_min
	if (!Overlap(vol1_min, vol1_max, vol2_min, vol2_max)) return false;

	

	return true;
}

sf::Vector2f Collision::relativePosition(GameObject* center, GameObject* object)
{
	return -(center->getTransform().transformPoint(center->getSize() * 0.5f) - object->getTransform().transformPoint(object->getSize() * 0.5f));
}

sf::Vector2f Collision::relativePosition(GameObject* center, sf::Vector2f point)
{
	return -(center->getTransform().transformPoint(center->getSize() * 0.5f) - point);
}

sf::Vector2f Collision::ShortestDistanceToLine(sf::Vector2f line1, sf::Vector2f line2, sf::Vector2f point)
{
	float length = Vector::magnitude(line2 - line1);

	float quot = Vector::dot(point - line1, line2 - line1) / pow(length, 2);

	return point - sf::Vector2f(line1.x+quot*(line2.x-line1.x),line1.y+quot*(line2.y-line1.y));
}

