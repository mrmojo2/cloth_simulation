#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle{
public:
	int radius;
	Vec2 prevPosition;
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;

	Vec2 netForce=Vec2(0.0,0.0);
	float mass;
	float invMass;

	void integrate(float dt);		//performs euler integration
	void addForce(const Vec2& f);
	
	Particle();
	Particle(int x, int y, float mass);
};

#endif
