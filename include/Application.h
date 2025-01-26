#ifndef APPLICATION_H
#define APPLICATION

#include "Particle.h"
#include "Spring.h"
#include "Vector.h"

#include <vector>
#include <SDL2/SDL.h>


class Application{
private:
	bool running = false;
	std::vector<Particle*> particles;
	std::vector<Spring> springMassSystems;
	Vec2 pushForce = Vec2(0,0);		//to give a force through keyboard
	Vec2 mousePos = Vec2(0,0);		
	
	bool drawMouseImpulseLine = false;		//to give a force to a particle using mouse
	int mouseImpulseParticleIndex;

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
