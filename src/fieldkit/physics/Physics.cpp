/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/physics/Physics.h"

using namespace fieldkit::physics;

Physics::Physics(Space* space) 
{
	this->space = space;
	
	numParticles = 0;
	numAllocated = 0;

	setParticleAllocator(new ParticleAllocator());
	setParticleUpdate(new ParticleUpdate());
	setSpringUpdate(new SpringUpdate());
	setNeighbourUpdate(new EmptyStrategy());
}

Physics::~Physics() 
{
	destroy();
}

void Physics::destroy()
{
	// strategies
	if(particleAllocator != NULL)
		delete particleAllocator;

	if(particleUpdate != NULL)
		delete particleUpdate;

	if(springUpdate != NULL)
		delete springUpdate;

	if(neighbourUpdate != NULL)
		delete neighbourUpdate;

	destroySprings();
	destroyParticles();

	// emitter
	delete emitter;
}

void Physics::update(float dt)
{
	if(emitter)
		emitter->update(dt);
	
	particleUpdate->apply(this);
	springUpdate->apply(this);
	neighbourUpdate->apply(this);
}

// -- Particles ----------------------------------------------------------------
// check if we still have a dead particle that we can reuse, otherwise create a new one
Particle* Physics::createParticle() 
{
	numParticles++;
	BOOST_FOREACH(Particle* p, particles) {
		if(!p->isAlive) return p;
	}
	
	particleAllocator->apply(this);
	return particles.back();
}

// allocates a bunch of new particles
void Physics::allocParticles(int count) 
{
	numAllocated = particles.size() + count;
	space->reserve(numAllocated);
	particles.reserve(numAllocated);
	
	for(int i=0; i<count; i++)
		particleAllocator->apply(this);
}

void Physics::addParticle(Particle* particle)
{
	particles.push_back(particle);
}

void Physics::destroyParticles()
{
	BOOST_FOREACH(Particle* p, particles) {
		if(p != NULL) {
			delete p;
			p = NULL;
		}
	}
	particles.clear();
}


// -- Springs ------------------------------------------------------------------
void Physics::addSpring(Spring* spring) 
{
	springs.push_back(spring);
}

void Physics::removeSpring(Spring* spring)
{
	// TODO
//	springs.erase(spring);
}

void Physics::destroySprings()
{
	BOOST_FOREACH(Spring* s, springs) {
		delete s;
		s = NULL;
	}
	springs.clear();
}