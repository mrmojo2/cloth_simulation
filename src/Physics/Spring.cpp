#include "Spring.h"

Spring::Spring(Particle* end1, Particle* end2,float stiffness) : end1(end1),end2(end2),stiffness(stiffness){
	eqb_length = (end1->position-end2->position).magnitude();	
}

