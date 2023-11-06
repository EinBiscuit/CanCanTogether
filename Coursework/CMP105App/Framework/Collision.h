// Collision Class
// Provides default functions for testing collision detection. Functions for standard axis aligned bounding box, and bounding circle.
// @author Paul Robertson

#pragma once
#include "GameObject.h"
#include "Vector.h"


// Static class provide collision detection functions.
class Collision
{
public:
	// Check AABB for collision. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* sp1, GameObject* sp2);
	// Check collision with box and point. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* s1, sf::Vector2i s2);
	// Check bounding circle collision. Returns true if collision occurs.
	static bool checkBoundingSphere(GameObject* sp1, GameObject* sp2);

	// E N H A N C E

	static bool checkOBBvsOBB(GameObject* sp1, GameObject* sp2);

	static bool checkCirclevsOBB(GameObject* sp1, GameObject* sp2);

	// return a vector from a center that points in dirrection of objects center
	static sf::Vector2f relativePosition(GameObject* center, GameObject* object ); 

	// return a vector from a center that points in dirrection of a point 
	static sf::Vector2f relativePosition(GameObject* center, sf::Vector2f point);

	//returns normal to a line from a specified point // persuming line is infinite
	static sf::Vector2f ShortestDistanceToLine(sf::Vector2f line1,sf::Vector2f line2, sf::Vector2f point);
};