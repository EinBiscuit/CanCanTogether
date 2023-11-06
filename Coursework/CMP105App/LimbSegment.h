#pragma once
#include "Framework/GameObject.h"
#include <iostream>
#include <math.h>
class LimbSegment :
	public GameObject
{
protected:
	//when constructed is getting attached to the master object ATTACHMENT POINT (point that is either origin (default) or relative to it), transfoms accordig to it
	GameObject* master;
	sf::Vector2f AttachmentPoint; // relative to local coordinates of master // masters origin point

	//ROTATION

	bool rot_independ = false;

	//conditionality of angle
	bool hinged = false;
	bool locked = false;

	//within which it permits the wobbling
	float upperHinge = 0; //+
	float lowerHinge = 0; //-

	float counter = 0; // counts master rotation

	//texture rectangles
	sf::IntRect Texture_dflt; // need to be set up with texture
	sf::IntRect Texture_Flip;
	

public:

	//constructors
	LimbSegment();

	~LimbSegment();

	LimbSegment(GameObject* master);

	LimbSegment(GameObject* master, sf::Vector2f m_local);

	//overrides

	void setFlipped(bool F);

	void rotate(float angle);

	void update(float dt);

	//etc

	sf::Vector2f getAttachmentPoint() { return AttachmentPoint; };

	void setMaster(GameObject* master) { this->master = master; };

	GameObject* getMaster() { return master; };

	void AttachToPoint(float m_local_x, float m_local_y) { AttachmentPoint = sf::Vector2f(m_local_x,m_local_y); };

	void AttachToPoint(sf::Vector2f AP) { AttachmentPoint = AP; };

	void setHinge(float inClockwise, float inCounterclockwise) { hinged = true;  upperHinge = inClockwise;  lowerHinge = inCounterclockwise; }; // how much can the master can turn before the slave will start following

	void setHinged(bool H) { hinged = H; };

	void setRotaryIndependence(bool i) { rot_independ = i; };
	
	void setLocked(bool L) { locked = L; };

	void setLimbTexture(sf::Texture* texture) { setTexture(texture); Texture_dflt = getTextureRect(), Texture_Flip = sf::IntRect(getTextureRect().width, 0, -getTextureRect().width, getTextureRect().height); }

};

