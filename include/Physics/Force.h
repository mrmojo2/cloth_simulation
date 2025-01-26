#ifndef FORCE_H
#define FORCE_H

#include "Particle.h"
#include "Vector.h"
#include "Spring.h"

class Force{
public:
	static Vec2 getDragForce(const Particle& p, float drag_coefficient);
	static Vec2 getFrictionalForce(const Particle& p, float friction_coefficient);
	static Vec2 getGravitationalForce(const Particle& p1, const Particle& p2, float gravitational_const, float min_distance, float max_distance);
	static Vec2 getSpringForce(const Spring& s);
};

#endif
