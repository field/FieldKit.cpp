/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Emitter.h
 *	 Created by Marcus Wendt on 20/05/2010.
 */
#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Behavioural.h"

namespace fieldkit { namespace physics {
		
	// FWD
	class Physics;

	class Emitter : public Behavioural {
	public:
		Physics* physics;
		
		Emitter(Physics* physics);
		virtual ~Emitter();
		
		void update(float dt);
		Particle* emit(Vec3f const& location);
		
		void setPosition(Vec3f const& location) { position.set(location); }
		Vec3f getPosition() { return position; }
		
		void setRate(int value) { rate = value; }
		int getRate() { return rate; }
		
		void setMax(int value);
		int getMax() { return max; }
		
		void setInterval(float value) { interval = value; }
		float getInterval() { return interval; }
		
		void setEnabled(bool value) { isEnabled = value; }
		bool getIsEnabled() { return isEnabled; }
		
	protected:
		Vec3f position;
		
		int rate;
		int max;
		float interval;
		float time;
		bool isEnabled;
	};
	
} } // namespace fieldkit::physics