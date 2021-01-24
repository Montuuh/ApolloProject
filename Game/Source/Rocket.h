#ifndef __ROCKET_H__
#define __ROCKET_H__

#include "Body.h"
#include "Vectors.h"
#include "Animation.h"

class Rocket : public Body
{

public:
	float fuel;
	Vec2 velocity;
	Vec2 acceleration;
	float angle;

	float maxVel = 300.0f;
	float velX = 0;
	float velY = 0;
public:


	void AddMomentum(float xV, float yV);
	void AddMomentumAngle(float xV, float yV, float angleRotation);


};



#endif // __ROCKET_H__