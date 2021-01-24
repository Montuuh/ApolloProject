#include "Rocket.h"
#include "PhysicsEngine.h"
#include "Module.h"

#include "Defs.h"
#include "Log.h"

Rocket::~Rocket()
{
	//idlAnim.PushBack({ 0, 0, 64, 85 });
}

void Rocket::launchRocket()
{

}
//
//bool Rocket::Awake()
//{
//	LOG("Loading Rocket");
//	bool ret = true;
//
//	return ret;
//}
//
//bool Rocket::Start()
//{
//	currentAnimation = &idlAnim;
//	return true;
//}
//
//
//bool Rocket::Update()
//{
//
//	return true;
//}
//
//bool Rocket::PostUpdate()
//{
//	SDL_Rect rect = currentAnimation->GetCurrentFrame();
//	return true;
//}


void Rocket::refillFuel()
{

}

void Rocket::AddMomentum(float vx, float vy)
{
	float velocityX = vx / mass;
	float velocityY = vy / mass;
	velocity.x += velocityX;
	velocity.y += velocityY;

}

void Rocket::AddMomentumAngle(float vx, float vy, float angleRot)
{
	velX = 0;
	velY = 0;



	if (angleRot >= 0 && angleRot < 90)
	{
		if (angleRot != 0)
		{
			velX = (vx + angleRot / mass) * 0.003;
		}
		else {
			velX = 0;
		}
		velY = (vy / mass + (-90 + angleRot)) * 0.003;

	}
	else if (angleRot < 0 && angleRot >= -90)
	{
		velX = -((vx - angleRot / mass) * 0.003);
		velY = -(vy / mass+(90+angleRot))*0.003;
	}
	else if (angleRot >= 90 && (angleRot <= 180))
	{
		if (angleRot != 180)
		{
			velX = (vx + angleRot / mass) * 0.003;
		}
		else {
			velX = 0;
		}
		velY = (vy / mass+(-90 + angleRot))*0.003;
	}
	else if (angleRot < -90 && angleRot >= -180)
	{
		if (angleRot != -180)
		{
			velX = -((vx - angleRot / mass) * 0.003);
		}
		else
		{
			velX = 0;
		}
		velY = -(vy / mass + (90 + angleRot)) * 0.003;
	}
	
	velocity.x += velX;
	
	if (velocity.y > limitVelocityDown)
	{
		velocity.y = limitVelocityDown;
	}else if(velocity.y < limitVelocityUp)
	{
		velocity.y = limitVelocityUp;
	}
	else {
		velocity.y += velY;
	}
		

}
