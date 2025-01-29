#include "Rod.h"

Rod::Rod(Particle* p1, Particle* p2):p1(p1),p2(p2){
	length = (p1->position - p2->position).magnitude();
}
