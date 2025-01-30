#include "Application.h"
#include "Graphics.h"
#include "Constants.h"
#include "Force.h"

#include <iostream>
#include <stdint.h>

#define DRAG_RADIUS 50

bool Application::isRunning(){
	return running;
}

void Application::setup(){
	running = Graphics::OpenWindow();

#if 1
	int row = 15, col = 30;
	for(int i = 0; i<row; i++){
		for(int j = 0; j< col; j++){
			float mass = i == 0 ? 0.0: 2.0;
			Particle* p = new Particle((Graphics::windowWidth/2 - 30*col/2) + j*30, 100 + i*30,mass);
			p->radius = 2;
			particles.push_back(p);
		}
	}

	//connecting each particles with rod
	for(int j=0; j<col;j++){
		for(int i=0;i<row;i++){
			Particle* p1 = particles[col*i+j];
			if(i!=row-1){
				Particle* p2 = particles[col*(i+1)+j];
				Rod r1 = Rod(p1,p2);
				rods.push_back(r1);
			}
			if(j!=col-1){
				Particle* p3 = particles[col*i + j + 1];
				Rod r2 = Rod(p1,p3);	
				rods.push_back(r2);
			}
		}
	}
#endif
#if 0
	Particle* p1 = new Particle(Graphics::windowWidth/2,100,0.0);
	Particle* p2 = new Particle(Graphics::windowWidth/2,200,5.0);
	Particle* p3 = new Particle(Graphics::windowWidth/2,300,2.0);
	Rod r = Rod(p1,p2);
	Rod r2 = Rod(p2,p3);
	particles.push_back(p1);
	particles.push_back(p2);
	//particles.push_back(p3);
	rods.push_back(r);
	//rods.push_back(r2);
#endif
}

void Application::input(){
	static int buttonDownTime, buttonUpTime;
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				if(event.key.keysym.sym == SDLK_UP)
					pushForce.y = -100*PIXELS_PER_METER;
				if(event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = 100*PIXELS_PER_METER;
				if(event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = -100*PIXELS_PER_METER;
				if(event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = +100*PIXELS_PER_METER;
				if(event.key.keysym.sym == SDLK_s)
					isSimulationRunning = true;
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_UP)
					pushForce.y = 0;
				if(event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = 0;
				if(event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = 0;
				if(event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = 0;
				break;
			case SDL_MOUSEMOTION:
				mousePos.x = event.motion.x;
				mousePos.y = event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:{
				int curMouseX = event.motion.x;
				int curMouseY = event.motion.y;		
				if(event.button.button == SDL_BUTTON_LEFT){
					dragParticleIndex.clear();
					for(int i=0;i<particles.size();i++){
						if( (curMouseX > particles[i]->position.x - (particles[i]->radius+DRAG_RADIUS)) && (curMouseX < particles[i]->position.x + (particles[i]->radius+DRAG_RADIUS)) && 
						    (curMouseY > particles[i]->position.y - (particles[i]->radius+DRAG_RADIUS)) && (curMouseY < particles[i]->position.y + (particles[i]->radius+DRAG_RADIUS)) ){
							dragMouse = true;
							dragStart = mousePos;
							dragParticleIndex.push_back(i);
						}
					}
					buttonDownTime = SDL_GetTicks();
				}
				break;
			}
			
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					buttonUpTime = SDL_GetTicks();
					if(dragMouse){
						dragMouse= false;
						/*Vec2 impulseDir = (particles[dragParticleIndex[0]]->position - mousePos).unit();
						float impulseMag = (particles[dragParticleIndex[0]]->position - mousePos).magnitude();
						particles[dragParticleIndex[0]]->addForce(impulseDir*impulseMag*100);*/
					}else{
						//Particle* p = new Particle(event.motion.x, event.motion.y, (buttonUpTime-buttonDownTime)/50);
						//particles.push_back(p);
					}
				}
				break;
				


		}
	}
}
void Application::update(){
	//maintain constant fps
	static int  previousFrameTime; 
	int delay = MILLISECONDS_PER_FRAME - (SDL_GetTicks()-previousFrameTime);
	if(delay>0){				//delay can be negative if a frame loop take more time than MILLISECONDS_PER_FRAME
		SDL_Delay(delay);
	}
	
	//delta time: difference between the current frame and the last frame in seconds
	//used to implement fps independant movement
	float deltaTime = (SDL_GetTicks() - previousFrameTime)/1000.0f;
	if(deltaTime > MILLISECONDS_PER_FRAME/1000.0f){
		deltaTime = MILLISECONDS_PER_FRAME/1000.0f;
	}	

	//set time of this frame to be used in the next frame
	previousFrameTime = SDL_GetTicks();
if(isSimulationRunning){
	//apply forces to the particles
	for(auto& particle:particles){
		//weight force
		particle->addForce(Vec2(0.0,9.8*PIXELS_PER_METER*particle->mass));
		
		//pushForce from keyboard
		particle->addForce(pushForce);
		
		//drag force
		Vec2 drag = Force::getDragForce(*particle, 0.002);
		//particle->addForce(drag);

	}

	//apply spring force
	for(auto& s:springMassSystems){
		Vec2 springForce = Force::getSpringForce(s);
		s.end1->addForce(springForce);
		s.end2->addForce(-springForce);
	}


	//perform integration
	for(auto& particle: particles){
		particle->integrate(deltaTime);
	}
	
	//apply rod constraint
	for(auto& rod:rods){
		Particle* A = rod.p1;
		Particle* B = rod.p2;
		if(A->invMass == 0 && B->invMass == 0) continue;
		Vec2 OA = A->position;
		Vec2 OB = B->position;
		
		Vec2 AB = OB-OA;
		float diff = AB.magnitude() - rod.length;
		Vec2 diff_vec = AB.unit()*diff;	
		
		float da = A->invMass / (A->invMass + B->invMass);
		float db = B->invMass / (A->invMass + B->invMass);

		A->position += diff_vec*da*0.8;	//THIS DAMPING FACTOR IS VERY VERY IMPORTANT FOR STABILITY!!!!
		B->position -= diff_vec*db*0.8;
	}

	
}
	//window boundary
	//TODO: handle the case of particle exceding the boundary!
	for(auto& particle:particles){
		if((particle->position.y > (Graphics::windowHeight - particle->radius))){
			//particle->position.y = Graphics::windowHeight - particle->radius;	//putting particle on the edges if it exceds
			//making the collision not perfectly elastic
				//particle->velocity.y *= -0.9;					//for euler integration

			Vec2 tmp = particle->prevPosition;
			particle->prevPosition.y = particle->position.y;
			particle->position.y = tmp.y;
		}else if(particle->position.y < (0 + particle->radius)){
			//particle->position.y = particle->radius;
			//particle->velocity.y *= -0.9;
			Vec2 tmp = particle->prevPosition;
			particle->prevPosition.y = particle->position.y;
			particle->position.y = tmp.y;
		}
		if(particle->position.x > (Graphics::windowWidth - particle->radius)){
			//particle->position.x = Graphics::windowWidth - particle->radius;	//putting particle on the edges if it exceds
			//particle->velocity.x *= -0.9;						//making the collision not perfectly elastic
			Vec2 tmp = particle->prevPosition;
			particle->prevPosition.x = particle->position.x;
			particle->position.x = tmp.x;
		}else if(particle->position.x < (0 + particle->radius)){
			//particle->position.x = particle->radius;
			//particle->velocity.x *= -0.9;
			Vec2 tmp = particle->prevPosition;
			particle->prevPosition.x = particle->position.x;
			particle->position.x = tmp.x;
		}	
	}
}
void Application::render(){
	Graphics::ClearScreen(0xFF112233);
	

	if(dragMouse){
		for(auto i:dragParticleIndex){
			particles[i]->position = particles[i]->position*0.8+ mousePos*0.2; //intrapolation for smooth dragging
		}

	}

	//render springs
	for(auto sm:springMassSystems){
		Graphics::DrawLine(sm.end1->position.x, sm.end1->position.y,sm.end2->position.x, sm.end2->position.y,0xffffffff);
	}	
	//render rods
	for(auto rod:rods){
		Graphics::DrawLine(rod.p1->position.x, rod.p1->position.y,rod.p2->position.x, rod.p2->position.y,0xffffffff);
	}	
	//render the particcles
	for(auto particle:particles)
		if(particle->mass == 0){
			Graphics::DrawFillCircle(particle->position.x,particle->position.y,2,0xff0000ff);		
		}else{
			Graphics::DrawFillCircle(particle->position.x,particle->position.y,particle->radius,0xffffffff);
		}
	Graphics::RenderFrame();	
}
void Application::destroy(){
	for(auto particle:particles){
		delete particle;
	}
	Graphics::CloseWindow();
}
