#include "Particle.h"

#include <algorithm>

Particle::Particle():radius(0),position(Vec2(0,0)),mass(0){}

Particle::Particle(int x, int y, float mass):position(Vec2(x,y)),mass(mass){
	radius = std::clamp((int)mass,5,20);
	if(mass)
		invMass = 1/mass;
	else
		invMass = 0;
}

void Particle::integrate(float dt){
	if(!invMass) return;
	acceleration = netForce * invMass;

	velocity += acceleration * dt;
	position += velocity * dt;

	//make netfor 0 for next frame
	netForce = Vec2(0,0);
	
	
	/*acceleration = netForce * invMass;

	if (!initialized) {
		// Initialize previous position for the first frame
		prevPosition = position;
		initialized = true;
	}

	 // Calculate the new position using Verlet integration
	 Vec2 newPosition = position + (position - prevPosition) + acceleration * (dt * dt);

	 // Update velocity for diagnostic or external use
	 velocity = (newPosition - prevPosition) / (2 * dt);

	 // Update positions for the next frame
	 prevPosition = position;
	 position = newPosition;

	 // Reset net force for the next frame
	  netForce = Vec2(0, 0);
	  */
}

void Particle::addForce(const Vec2& f){
	netForce += f;
}
