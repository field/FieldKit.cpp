/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Spring.cpp
 *	 Created by Marcus Wendt on 27/05/2010.
 */

#include "fieldkit/physics/Spring.h"

namespace fk { namespace physics {
	
	// Constructors
	Spring::Spring() {
		restLength = 0;
		strength = 0;
		isALocked = false;
		isBLocked = false;
	}
	
	Spring::Spring(ParticlePtr a, ParticlePtr b, float restLength, float strength) {
		this->a = a;
		this->b = b;
		this->restLength = restLength;
		this->strength = strength;
	}
	
	void Spring::update() {
		Vec3f delta = b->position - a->position;
		float dist = delta.length() + EPSILON_VALUE;
		float normDistStrength = (dist - restLength) / (dist * (a->invWeight + b->invWeight)) * strength;
		
		if(!a->isLocked && !isALocked) {
			a->position += delta * normDistStrength * a->invWeight;
		}
		
        if(!b->isLocked && !isBLocked) {
			b->position += delta * -normDistStrength * b->invWeight;
		}
	}
	
} } // fk::physics