/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/07/2010.
 */

#pragma once

#include "fieldkit/physics/space/Spatial.h"
#include "fieldkit/physics/space/Space.h"

namespace fieldkit { namespace physics {

	//! A 2D spatial hashing class used to efficiently retrieve large numbers of 
	//! uniformly distributed Spatials.
	class SpatialHash : public Space {
	public:
		
		class Cell {
		public:			
			Cell();
			~Cell();
			void clear();
			void insert(SpatialPtr spatial);
			SpatialList spatials;
			bool isEmpty;
		};

		typedef Cell* CellPtr;
		typedef CellPtr* CellList;
		typedef CellList* CellArray;
		
		//! Constructs a new SpatialHash.
		SpatialHash();
		SpatialHash(Vec3f offset, Vec3f dimension, float cellSize=5.0f);
		~SpatialHash();
		
		void init(Vec3f offset, Vec3f dimension, float cellSize=5.0f);
		
		//! Empties the entire space contents.
		void clear();
		
		//! Adds a new spatial into the hash.
		void insert(SpatialPtr spatial);
		
		//! Selects all spatials within the given bounding volume.
		void select(BoundingVolumePtr volume, SpatialListPtr result);
		
	protected:
		CellArray cells;
		int cellsX;
		int cellsY;
		float cellSize;
		
		void destroy();
	};
	
} } // namespace fieldkit::physics