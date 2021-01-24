#pragma once

#include "App.h"
#include "Module.h"
#include "Vectors.h"
#include "Rocket.h"
#include "Planet.h"
#include "Body.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class PhysicsEngine : public Module
{
public:

    PhysicsEngine();
    virtual ~PhysicsEngine();    

public:

    bool Start();
    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();
    bool CleanUp();

public: 

	Rocket* CreateRocket(Vec2 position, float mass, Vec2 velocity, float angle);

    void Verlet(Vec2* pos, Vec2* v, Vec2 a, float dt);

    void ApplyGravity();

    Vec2 gravityEarth;
    Vec2 gravityMoon;
    Rocket *rocket;

};