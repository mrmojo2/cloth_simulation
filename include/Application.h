#ifndef APPLICATION_H
#define APPLICATION

#include "Particle.h"
#include "Spring.h"
#include "Rod.h"
#include "Vector.h"

#include <vector>
#include <SDL2/SDL.h>


class Application{
private:
	bool running = false;
	bool isSimulationRunning = false;
	
	std::vector<Particle*> particles;
	std::vector<Spring> springMassSystems;
	std::vector<Rod> rods;
	
	Vec2 pushForce = Vec2(0,0);		//to give a force through keyboard
	
	Vec2 mousePos = Vec2(0,0);
	Vec2 dragStart = Vec2(0,0);	
	bool dragMouse = false;	
	std::vector<int> dragParticleIndex;

public:
	Application() = default;
	~Application() = default;

	bool isRunning();
	void setup();
	void input();
	void update();
	void render();
	void destroy();
};


#endif
