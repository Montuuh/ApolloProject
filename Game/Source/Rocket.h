#ifndef __ROCKET_H__
#define __ROCKET_H__

#include "Body.h"
#include "Vectors.h"
#include "Animation.h"

class Rocket : public Body
{
public:

	Rocket(){}
	~Rocket();

public:
	int health;
	float fuel;
	Vec2 velocity;
	Vec2 acceleration;
	float f;
	float angle;
	float limitVelocityUp = -300.0f;
	float limitVelocityDown = 300.0f;
	float velX;
	float velY;
public:

	void launchRocket();
	void refillFuel();
	void AddMomentum(float xV, float yV);
	void AddMomentumAngle(float xV, float yV, float angleRotation);


};



#endif // __ROCKET_H__