#ifndef SPRING_H
#define SPRING_H

#include "Particle.h"

class Spring{
public:
	Particle* end1;
	Particle* end2;
	float eqb_length;
	float stiffness;

	Spring(Particle*, Particle*, float stiffness);
};


#endif
