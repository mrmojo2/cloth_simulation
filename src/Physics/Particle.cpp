#include "Particle.h"

#include <algorithm>

Particle::Particle():radius(0),position(Vec2(0,0)),mass(0){}

Particle::Particle(int x, int y, float mass):position(Vec2(x,y)),mass(mass){
	radius = std::clamp((int)mass,5,20);
	prevPosition = position;

	if(mass)
		invMass = 1/mass;
	else
		invMass = 0;
}

void Particle::integrate(float dt){
	if(!invMass) return;
	acceleration = netForce * invMass;
	
	///////////////////////////////////////////////////////////////////////////////	
	//Euler Integration
	///////////////////////////////////////////////////////////////////////////////	
	/*velocity += acceleration * dt;
	position += velocity * dt;*/

	///////////////////////////////////////////////////////////////////////////////	
	//Verlet Integration
	///////////////////////////////////////////////////////////////////////////////	
	
	Vec2 pos = position;
	position.x = 2*position.x - prevPosition.x + acceleration.x*dt*dt;
	position.y = 2*position.y - prevPosition.y + acceleration.y*dt*dt;
	prevPosition = pos;
	
	//make netforce 0 for next frame
	netForce = Vec2(0,0);
	
	
}

void Particle::addForce(const Vec2& f){
	netForce += f;
}
