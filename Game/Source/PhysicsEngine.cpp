#include "PhysicsEngine.h"
#include <math.h>


PhysicsEngine::PhysicsEngine() : Module()
{

}

PhysicsEngine::~PhysicsEngine()
{
}

bool PhysicsEngine::Start()
{
	gravityEarth = Vec2(0, 0.18f);
	gravityMoon = Vec2(0, 0.01f);
	return true;
}

bool PhysicsEngine::PreUpdate()
{
	return true;
}

bool PhysicsEngine::Update(float dt)
{
	Verlet(&rocket->pos, &rocket->velocity, rocket->acceleration, dt);

	ApplyGravity();

	return true;
}

bool PhysicsEngine::PostUpdate()
{
	return true;
}

bool PhysicsEngine::CleanUp()
{
	return true;
}

void PhysicsEngine::Verlet(Vec2 *pos, Vec2 *v, Vec2 a, float dt)
{
	v->x = v->x + a.x * pow(dt, 2);
	v->y = v->y + a.y * pow(dt, 2);

	pos->x = pos->x + v->x * dt + 0.5 * a.x * pow(dt, 2);
	pos->y = pos->y + v->y * dt + 0.5 * a.y * pow(dt, 2);

}

void PhysicsEngine::ApplyGravity()
{
	if (rocket->pos.y > -2000)				// Apply gravity if rocket is in earth atmosphere
		rocket->velocity += gravityEarth;  
	else if (rocket->pos.y < -8500)			// Apply gravity if rocket is in moon atmosphere
		rocket->velocity -= gravityMoon;
	else;									// Space, do not apply gravity
	
}

Rocket* PhysicsEngine::CreateRocket(Vec2 position, float mass, Vec2 velocity, float angle)
{
	Rocket *rocket = new Rocket();
	rocket->pos = position;
	rocket->mass = mass;
	rocket->velocity = velocity;
	rocket->angle = angle;

	return rocket;
}

