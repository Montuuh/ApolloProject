#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Body.h"
#include "PhysicsEngine.h"



#include "Defs.h"
#include "Log.h"


Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	backgroundTex = app->tex->Load("Assets/Textures/background_space.png");  // Background parallax texture
	winTex = app->tex->Load("Assets/Textures/winText.png");					 // Intro screen texture
	loseTex = app->tex->Load("Assets/Textures/loseText.png");				 // Lose screen texture
	introTex = app->tex->Load("Assets/Textures/introText.png");				 // Win screen texture

	rocketTex = app->tex->Load("Assets/Textures/rocket3.png");				  // Rocket texture

	fullFuelTex = app->tex->Load("Assets/Textures/fullfuel.png");			  // Full fuel texture 
	threeFuelTex = app->tex->Load("Assets/Textures/threefuel.png");			  // 3 fuel left texture
	twoFuelTex = app->tex->Load("Assets/Textures/twofuel.png");				  // 2 fuel left texture
	oneFuelTex = app->tex->Load("Assets/Textures/onefuel.png");				  // 1 fuel left texture
	emptyFuelTex = app->tex->Load("Assets/Textures/nofuel.png");			  // Empty fuel texture
	fuelFillerTex = app->tex->Load("Assets/Textures/itemBattery.png");		  // Fuel filler texture

	deadTex = app->tex->Load("Assets/Textures/explode2.png");				  // Dead texture

	app->audio->PlayMusic("Assets/Audio/Music/earth_scene.ogg");
	
	app->physicsEngine->rocket = app->physicsEngine->CreateRocket(Vec2(622, 520), 10, Vec2(0,0), 0.0f); // Position, mass, velocity, angle

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && !gameStarted) // If intro screen and enter pressed -> GAME
	{
		gameStarted = true;
	}

	// Water logic and animation
	{
		if ((app->physicsEngine->rocket->pos.x <= 384 || app->physicsEngine->rocket->pos.x >= 895) && // X position of the sea
			app->physicsEngine->rocket->pos.y > 480 &&								                  // Y position of the sea
			waterAnimCount >= 100)																	  // Counter to make a little water animation
		{
			waterAnimCount = 0;
			if (app->physicsEngine->rocket->velocity.y > 1000)
			{
				app->physicsEngine->rocket->velocity.y = 1000;
			}
			app->physicsEngine->rocket->velocity.y -= 50;
			waterAnim = true;
		}
		else if (app->physicsEngine->rocket->pos.y <= 480 && waterAnim) //  If the rocket has rised up from the water again, die
		{
			app->physicsEngine->rocket->velocity.y = 0;
			hasDead = true;
		}
		waterAnimCount++;
	}
	
	if (gameStarted == true && !isDead)
	{
		// Lose conditions
		{
			// Die at earth condition
			if (app->physicsEngine->rocket->pos.y > 480 &&
				app->physicsEngine->rocket->pos.x >= 384 && app->physicsEngine->rocket->pos.x <= 895 &&
				(app->physicsEngine->rocket->angle > 30 || app->physicsEngine->rocket->angle < -30))
			{
				hasDead = true;
			}

			// Die at moon condition
			if (app->physicsEngine->rocket->pos.y <= -10449 &&
				app->physicsEngine->rocket->angle < 150 && app->physicsEngine->rocket->angle > -150)
			{
				hasDead = true;
			}
		}
	
		// Angles corrections when rocket still on ground
		{
			// Correct the angle at earth	
			if (app->physicsEngine->rocket->pos.y >= 480 &&													// Y position of the ground
				app->physicsEngine->rocket->pos.x >= 384 && app->physicsEngine->rocket->pos.x <= 895 &&     // X position of the ground
				app->physicsEngine->rocket->angle < 30 && app->physicsEngine->rocket->angle > -30 &&		// Correct the angle between 30º to both sides
				app->physicsEngine->rocket->angle != 0)														// If angle = 0 is correct.
			{
				if (angleCorrectionAnimCounter > 80)  // Counter to make an little animation
				{
					if (app->physicsEngine->rocket->angle > 1)	// If the angle is positive, iterate the angle till its 0
					{
						app->physicsEngine->rocket->angle--;
					}
					else if (app->physicsEngine->rocket->angle < -1)	// If the angle is negative, iterate the angle till its 0
					{
						app->physicsEngine->rocket->angle++;
					}
					else
					{
						app->physicsEngine->rocket->angle = 0;
					}
					angleCorrectionAnimCounter = 0;
				}
				angleCorrectionAnimCounter++;
			}

			// Correct the angle at moon
			if (app->physicsEngine->rocket->pos.y <= -10450 &&												// Y position of the moon ground
				(app->physicsEngine->rocket->angle < -150 || app->physicsEngine->rocket->angle > 150) &&    // Correct the angle between 30º(as its upsidedown the angles are 150º) to both sides
				app->physicsEngine->rocket->angle != 0)														// If angle = 0 is correct.
			{
				if (angleCorrectionAnimCounter > 80) // Counter to make an little animation
				{
					if (app->physicsEngine->rocket->angle < 179 && app->physicsEngine->rocket->angle > 150) // If the angle is positive, iterate the angle till its 180
					{
						app->physicsEngine->rocket->angle++;
					}
					else if (app->physicsEngine->rocket->angle > -179 && app->physicsEngine->rocket->angle < -150) // If the angle is negative, iterate the angle till its 180
					{
						app->physicsEngine->rocket->angle--;
					}
					else
					{
						app->physicsEngine->rocket->angle = 180;
					}
					angleCorrectionAnimCounter = 0;
				}
				angleCorrectionAnimCounter++;
			}
		}

		// Ground "colliders"
		{
			if (app->physicsEngine->rocket->pos.y < -10450) // Moon
			{
				app->physicsEngine->rocket->pos.y = -10450;
				app->physicsEngine->rocket->velocity.y = 0;
				app->physicsEngine->rocket->velocity.x = 0;

			}
			if (app->physicsEngine->rocket->pos.y > 480 && // Earth
				!hasDead &&
				app->physicsEngine->rocket->pos.x >= 384 && app->physicsEngine->rocket->pos.x <= 895) // At the ground, not sea
			{
				app->physicsEngine->rocket->pos.y = 480;
				app->physicsEngine->rocket->velocity.y = 0;
				app->physicsEngine->rocket->velocity.x = 0;
			}
		}
		
		// Rocket limits
		{
			if (app->physicsEngine->rocket->pos.x > 1244)
			{
				app->physicsEngine->rocket->pos.x = 1244;
				app->physicsEngine->rocket->velocity.x = 0;
			}
			if (app->physicsEngine->rocket->pos.x < 0)
			{
				app->physicsEngine->rocket->pos.x = 0;
				app->physicsEngine->rocket->velocity.x = 0;
			}
		}

		// Fuel logic
		// Fuel velocity consumption
		{
			if (counter <= 37500) // Full fuel, the higher the counter, the less fuel consumption
			{
				fullFuel = true;
				fuel3 = false;
				fuel2 = false;
				fuel1 = false;
				emptyFuel = false;
			}
			if (counter > 37500 && counter <= 75000)  // 3 fuel left
			{
				fullFuel = false;
				fuel3 = true;
				fuel2 = false;
				fuel1 = false;
				emptyFuel = false;
			}
			if (counter > 75000 && counter <= 112500) // 2 fuel left
			{
				fullFuel = false;
				fuel3 = false;
				fuel2 = true;
				fuel1 = false;
				emptyFuel = false;
			}
			if (counter > 112500 && counter <= 150000) // 1 fuel left
			{
				fullFuel = false;
				fuel3 = false;
				fuel2 = false;
				fuel1 = true;
				emptyFuel = false;
			}
			if (counter > 150000 && counter < 187500) // no fuel left
			{
				fullFuel = false;
				fuel3 = false;
				fuel2 = false;
				fuel1 = false;
				emptyFuel = true;
			}
		}

		// Fuel fillers
		{
			if (hasFuelFiller1Taken && !fuelFiller1Bool)
			{
				if (emptyFuel)
				{
					emptyFuel = false;
					fuel1 = true;
					counter = 112500;
				}
				if (fuel1)
				{
					fuel2 = true;
					fuel1 = false;
					counter = 75000;
				}
				if (fuel2)
				{
					fuel3 = true;
					fuel2 = false;
					counter = 37500;
				}
				if (fuel3)
				{
					fuel3 = true;
				}
				if (fullFuel)
				{
					fullFuel = true;
					counter = 0;
				}

				fuelFiller1Bool = true;
			}

			if (hasFuelFiller2Taken && !fuelFiller2Bool)
			{
				if (emptyFuel)
				{
					emptyFuel = false;
					fuel1 = true;
					counter = 112500;
				}
				if (fuel1)
				{
					fuel2 = true;
					fuel1 = false;
					counter = 75000;
				}
				if (fuel2)
				{
					fuel3 = true;
					fuel2 = false;
					counter = 37500;
				}
				if (fuel3)
				{
					fuel3 = true;
				}
				if (fullFuel)
				{
					fullFuel = true;
					counter = 0;
				}

				fuelFiller2Bool = true;
			}
			if (hasFuelFiller3Taken && !fuelFiller3Bool)
			{
				if (emptyFuel)
				{
					emptyFuel = false;
					fuel1 = true;
					counter = 112500;
				}
				if (fuel1)
				{
					fuel2 = true;
					fuel1 = false;
					counter = 75000;
				}
				if (fuel2)
				{
					fuel3 = true;
					fuel2 = false;
					counter = 37500;
				}
				if (fuel3)
				{
					fuel3 = true;
				}
				if (fullFuel)
				{
					fullFuel = true;
					counter = 0;
				}

				fuelFiller3Bool = true;
			}
			if (hasFuelFiller4Taken && !fuelFiller4Bool)
			{
				if (emptyFuel)
				{
					emptyFuel = false;
					fuel1 = true;
					counter = 112500;
				}
				if (fuel1)
				{
					fuel2 = true;
					fuel1 = false;
					counter = 75000;
				}
				if (fuel2)
				{
					fuel3 = true;
					fuel2 = false;
					counter = 37500;
				}
				if (fuel3)
				{
					fuel3 = true;
				}
				if (fullFuel)
				{
					fullFuel = true;
					counter = 0;
				}

				fuelFiller4Bool = true;
			}
		}

		//Rocket movement
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && !hasDead && !waterAnim)
			{
				app->physicsEngine->rocket->AddMomentumAngle(1.075f, -1.1f, app->physicsEngine->rocket->angle);
				counter++;
			}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && !hasDead && !waterAnim)
			{
				app->physicsEngine->rocket->angle -= 0.015f;
				if (app->physicsEngine->rocket->angle < -180)
				{
					app->physicsEngine->rocket->angle = 180;
				}

			}
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !hasDead && !waterAnim)
			{
				app->physicsEngine->rocket->angle += 0.015f;
				if (app->physicsEngine->rocket->angle > 180)
				{
					app->physicsEngine->rocket->angle = -180;
				}
			}

		// Fuel fillers "collision" detection
		{
			if (app->physicsEngine->rocket->pos.y > -3500 && app->physicsEngine->rocket->pos.y < -3380 && // Y coordinate range of collision
				app->physicsEngine->rocket->pos.x > 280 && app->physicsEngine->rocket->pos.x < 360)       // X coordinate range of collision
					hasFuelFiller1Taken = true;
			
			if (app->physicsEngine->rocket->pos.y > -5000 && app->physicsEngine->rocket->pos.y < -4880 && // Y coordinate range of collision
				app->physicsEngine->rocket->pos.x > 480 && app->physicsEngine->rocket->pos.x < 560)       // X coordinate range of collision
					hasFuelFiller2Taken = true;

			if (app->physicsEngine->rocket->pos.y > -6500 && app->physicsEngine->rocket->pos.y < -6380 && // Y coordinate range of collision
				app->physicsEngine->rocket->pos.x > 230 && app->physicsEngine->rocket->pos.x < 310)       // X coordinate range of collision
					hasFuelFiller3Taken = true;

			if (app->physicsEngine->rocket->pos.y > -8500 && app->physicsEngine->rocket->pos.y < -8380 && // Y coordinate range of collision
				app->physicsEngine->rocket->pos.x > 680 && app->physicsEngine->rocket->pos.x < 760)       // X coordinate range of collision
					hasFuelFiller4Taken = true;
		}
		
		// Camera movement
		if (app->physicsEngine->rocket->pos.y < 300 && !emptyFuel)
		{
			app->render->camera.y = -(app->physicsEngine->rocket->pos.y - 300);
		}
	}

	// Lose and win transition and reset level
	if (isDead || hasWon) // If lose or win screen
		{
			app->tex->UnLoad(deadTex);
			app->tex->UnLoad(rocketTex);
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) // Unload all textures if enter pressed, and reset the game
			{
				CleanUp();
				Restart();
			}
		}

	return true;
}


// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (!gameStarted) // Intro screen texture
	{
		app->render->DrawTexture(introTex, 0, 0);
	}

	if (gameStarted) // GAME
	{
		// Background level texture
		app->render->DrawTexture(backgroundTex, 0, -10780);

		if (!hasWon) 
		{
			// Rocket drawing
			app->render->DrawTexture(rocketTex, app->physicsEngine->rocket->pos.x, app->physicsEngine->rocket->pos.y, 0, 1.0, app->physicsEngine->rocket->angle);
			
			// Fuel drawing UI
			{
				if (fullFuel) app->render->DrawTexture(fullFuelTex, -(app->render->camera.x - 1100), -(app->render->camera.y - 550));
				if (fuel3) app->render->DrawTexture(threeFuelTex, -(app->render->camera.x - 1100), -(app->render->camera.y - 550));
				if (fuel2) app->render->DrawTexture(twoFuelTex, -(app->render->camera.x - 1100), -(app->render->camera.y - 550));
				if (fuel1) app->render->DrawTexture(oneFuelTex, -(app->render->camera.x - 1100), -(app->render->camera.y - 550));
				if (emptyFuel) app->render->DrawTexture(emptyFuelTex, -(app->render->camera.x - 1100), -(app->render->camera.y - 550));
			}
			// Fuel filler cans drawing
			{
				if (hasFuelFiller1Taken);
				else
					app->render->DrawTexture(fuelFillerTex, 300, -3500);

				if (hasFuelFiller2Taken);
				else
					app->render->DrawTexture(fuelFillerTex, 500, -5000);
				if (hasFuelFiller3Taken);
				else
					app->render->DrawTexture(fuelFillerTex, 250, -6500);
				if (!hasFuelFiller4Taken);
				else
					app->render->DrawTexture(fuelFillerTex, 700, -8500);
			}
		}
		
		// Win conditions and win screen transition
		{
			if (app->physicsEngine->rocket->pos.y <= -8500) // Has arrived to the moon safely
			{
				moonArrived = true;
			}
			if (moonArrived && app->physicsEngine->rocket->pos.y >= 480 && !hasDead) // Has arrived moon and returned to the earth ground
			{
				app->render->DrawTexture(winTex, 0, 0);
				hasWon = true;
			}
		}

		// Lose screen transition
		if (isDead) 
		{
			app->physicsEngine->rocket->pos.y = 0;
			app->render->camera.y = 0;
			app->render->DrawTexture(loseTex, 0, app->physicsEngine->rocket->pos.y);
		}

		// Lose empty fuel condition
		if (emptyFuel) 
		{
			app->physicsEngine->rocket->velocity.x = 0;
			app->physicsEngine->rocket->velocity.y = 0;

			app->tex->UnLoad(rocketTex); // Make the rocket disappear, as it doesn't explode but travels through space indefinitely

			app->render->DrawTexture(emptyFuelTex, -(app->render->camera.x - 600), -(app->render->camera.y - 250)); // Insert a empty fuel texture
			
			deadTransitionCounter++;
		}

		// Enters instanlty after a crash
		if (hasDead) 
		{
			app->physicsEngine->rocket->velocity.x = 0;
			app->physicsEngine->rocket->velocity.y = 0;
			app->tex->UnLoad(rocketTex);
			app->render->DrawTexture(deadTex, app->physicsEngine->rocket->pos.x, app->physicsEngine->rocket->pos.y);

			deadTransitionCounter++;
		}

		// A dead timer to make a little animation to die
		if (deadTransitionCounter >= 5000)
		{
			isDead = true;
		}
	}
	return ret;
}

bool Scene::Restart()
{
	Start();

	hasDead = false;
	fullFuel = false;
	fuel3 = false;
	fuel2 = false;
	fuel1 = false;
	emptyFuel = false;
	waterAnim = false;
	waterAnimCount = 0;
	angleCorrectionAnimCounter = 0;
	counter = 0;
	deadTransitionCounter = 0;

	app->render->camera.y = 0;
	app->physicsEngine->rocket->velocity.y = 0;
	app->physicsEngine->rocket->pos.x = 622;
	app->physicsEngine->rocket->pos.y = 461;
	app->physicsEngine->rocket-> angle = 0;

	gameStarted = true;
	hasWon = false;
	moonArrived = false;
	isDead = false;
	hasFuelFiller1Taken = false;
	hasFuelFiller2Taken = false;
	hasFuelFiller3Taken = false;
	hasFuelFiller4Taken = false;

	fuelFiller1Bool = false;
	fuelFiller2Bool = false;
	fuelFiller3Bool = false;
	fuelFiller4Bool = false;

	prova = false;

	return true;

}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	
	app->tex->UnLoad(backgroundTex);
	app->tex->UnLoad(rocketTex);
	app->tex->UnLoad(winTex);
	app->tex->UnLoad(loseTex);
	app->tex->UnLoad(introTex);
	app->tex->UnLoad(fullFuelTex);
	app->tex->UnLoad(threeFuelTex);
	app->tex->UnLoad(twoFuelTex);
	app->tex->UnLoad(oneFuelTex);
	app->tex->UnLoad(emptyFuelTex);
	app->tex->UnLoad(fuelFillerTex);
	app->tex->UnLoad(deadTex);


	return true;
}

