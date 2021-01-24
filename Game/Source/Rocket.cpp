#include "Rocket.h"
#include "PhysicsEngine.h"
#include "Module.h"

#include "Defs.h"
#include "Log.h"

void Rocket::AddMomentum(float vx, float vy)
{
	velX = vx / mass;
	velY = vy / mass;

	velocity.x += velX;
	velocity.y += velY;

}

void Rocket::AddMomentumAngle(float vx, float vy, float angleRot)
{
	// Angle quadrants logic
	if (angleRot >= 0 && angleRot < 90) // Quadrant between 0º and 90º
	{
		if (angleRot == 0)
			velX = 0;
		else
		{
			velX = vx + angleRot / mass;
			velX = velX * 0.01;
		}

		velY = vy / mass + -90 + angleRot;
		velY = velY * 0.01;

	}
	else if (angleRot >= 90 && (angleRot <= 180)) // Quadrant between 90º and 180º
	{
		if (angleRot == 180)
			velX = 0;
		else
		{
			velX = vx + angleRot / mass;
			velX = velX * 0.01;
		}

		velY = vy / mass + -90 + angleRot;
		velY = velY * 0.01;
	}
	else if (angleRot >= -180 && angleRot < -90) // Quadrant between -180º(180º) and -90º(270º)
	{
		if (angleRot == -180)
			velX = 0;
		else
		{
			velX = -(vx - angleRot / mass);
			velX = velX * 0.01;
		}

		velY = -(vy / mass + 90 + angleRot);
		velY = velY * 0.01;
	}
	else if (angleRot < 0 && angleRot >= -90) // Quadrant between -90º(270º) and 0º(360º)
	{
		velX = -(vx - angleRot / mass);
		velX = velX * 0.01;

		velY = -(vy / mass + 90 + angleRot);
		velY = velY * 0.01;
	}

	
	velocity.x += velX;
	velocity.y += velY;

	// Limiting velocities
	if (velocity.y > maxVel)      // Max velocity down
		velocity.y = maxVel;
	else if(velocity.y < -maxVel) // Max velocity up
		velocity.y = -maxVel;

}
