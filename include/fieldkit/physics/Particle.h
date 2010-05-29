/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Particle.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "fieldkit/physics/Spatial.h"
#include "fieldkit/physics/Behavioural.h"
#include "fieldkit/math/BoundingVolume.h"
#include "fieldkit/FieldKit.h"
#include <list>
using std::list;

namespace fk { namespace physics {

class Particle : public Vec3f, Spatial
{
	public:
		list<void *> neighbours;
		BoundingVolume *neighbourBound;
		
		int state;
		float age;
		float lifeTime;
		
		// flag set if particles lifeTime is up
		bool isAlive;
		
		// physical size in the simulation space
		float size;
		
		// used by Springs and some Behaviours
		float weight;
		
		// inverted weight
		float invWeight;
		
		// wether this particle can be affected by behaviours
		bool isLocked;
		
		// previous position in the simulation space
		Vec3f prev;
		
		// amount of friction
		float drag;
		
		// force accumulator applied to this particle (set to zero after each update)
		Vec3f force;
		
		Particle() {};
		~Particle() {};
		
		// lifecycle
		virtual void init(Vec3f location);
		virtual void update(float dt);
		
		virtual void updateState(float dt);
		virtual void updatePosition();
		Vec3f *getSpatialPosition(){ return this; };

		// verlet integration
		void lock();
		void unlock();
		void clearVelocity();
		void scaleVelocity(float s);
		
		// getters & setters
		void setWeight(float value);
		inline float getWeight() { return this->weight; };
		inline float getInvWeight() { return this->invWeight; };
};
	
} } // namespace fk::physics