#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Rocket.h"
#include "Planet.h"

#include "Defs.h"


struct SDL_Texture;
class SDL_Rect;


class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	bool Restart();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	
private:
	SDL_Texture* backgroundTex;
	SDL_Texture* rocketTex;
	SDL_Texture* rocketPushTex;
	SDL_Texture* winTex;
	SDL_Texture* loseTex;
	SDL_Texture* introTex;
	SDL_Texture* fullFuelTex;
	SDL_Texture* threeFuelTex;
	SDL_Texture* twoFuelTex;
	SDL_Texture* oneFuelTex;
	SDL_Texture* emptyFuelTex;
	SDL_Texture* emptyFuelTexBig;
	SDL_Texture* fuelFillerTex;
	SDL_Texture* deadTex;


	bool gameStarted = false;

	bool fullFuel = false;
	bool fuel1 = false;
	bool fuel2 = false;
	bool fuel3 = false;
	bool emptyFuel = false;

	int counter = 0;
	int deadTransitionCounter = 0;
	int angleCorrectionAnimCounter = 0;

	
	bool hasWon = false;
	bool moonArrived = false;

	bool isDead = false;
	bool hasDead = false;

	bool hasFuelFiller1Taken = false;
	bool hasFuelFiller2Taken = false;
	bool hasFuelFiller3Taken = false;
	bool hasFuelFiller4Taken = false;
	bool fuelFiller1Bool = false;
	bool fuelFiller2Bool = false;
	bool fuelFiller3Bool = false;
	bool fuelFiller4Bool = false;

	uint fuelFillerFx;
	uint deadFx;
	uint winFx;
	uint moonFx;
};

#endif // __SCENE_H__