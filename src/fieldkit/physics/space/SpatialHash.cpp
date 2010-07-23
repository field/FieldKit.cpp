/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/07/2010.
 */

#include "fieldkit/physics/space/SpatialHash.h"

#include "fieldkit/math/MathKit.h"
using namespace fieldkit::math;

using namespace fieldkit::physics;

// -- SpatialHash::Cell --------------------------------------------------------
SpatialHash::Cell::Cell() 
{
	isEmpty=true;
}

SpatialHash::Cell::~Cell() 
{
}

void SpatialHash::Cell::clear()
{
	spatials.clear();
	isEmpty=true;
}

void SpatialHash::Cell::insert(SpatialPtr spatial)
{
	spatials.push_back(spatial);
	isEmpty=false;
}


// -- SpatialHash --------------------------------------------------------------
SpatialHash::SpatialHash()
{
	cells = NULL;
	init(Vec3f::zero(), Vec3f(100.0f, 100.0f, 100.0f), 10.0f);	
}

SpatialHash::SpatialHash(Vec3f offset, Vec3f dimension, float cellSize)
{
	cells = NULL;
	init(offset, dimension, cellSize);	
}

SpatialHash::~SpatialHash()
{
	destroy();
}

void SpatialHash::init(Vec3f offset, Vec3f dimension, float cellSize)
{
	// init bounds
	this->position = offset + dimension;
	this->extent = dimension;
	updateBounds();

	// create cells
	this->cellSize = cellSize;
	cellsX = (int)(dimension.x / cellSize);
	cellsY = (int)(dimension.y / cellSize);

	if(cells != NULL) 
		destroy();
	
	cells = new CellList[cellsX];
	for(int i=0; i < cellsX; i++) {
		cells[i] = new CellPtr[cellsY];
		for(int j=0; j < cellsY; j++) {
			cells[i][j] = CellPtr(new Cell());
		}
	}
}

void SpatialHash::destroy()
{
	for(int i = 0; i < cellsX; i++) {
		for(int j = 0; j < cellsY; j++) {
			delete cells[i][j];
		}
		delete[] cells[i];
	}
	delete[] cells;
	cells = NULL;
}	

void SpatialHash::clear() 
{
	for(int i=0; i < cellsX; i++) {
		for(int j=0; j < cellsY; j++) {
			cells[i][j]->clear();
		}
	}	
}

void SpatialHash::insert(SpatialPtr spatial) 
{
	// find position in cell space
	Vec3f p = spatial->getPosition();
	int hashX = (int)(p.x / cellSize);
	int hashY = (int)(p.y / cellSize);

	// make sure the spatial lies within the cell space
	if(hashX > 0 && hashX < cellsX && 
	   hashY > 0 && hashY < cellsY) {
		cells[hashX][hashY]->insert(spatial);
	}
}

void SpatialHash::select(BoundingVolumePtr volume, SpatialListPtr result)
{
	// find search center position in cell space
	Vec3f p = volume->getPosition();
	int hashX = (int)(p.x / cellSize);
	int hashY = (int)(p.y / cellSize);
	
	// figure out search radius
	int searchX = 0;
	int searchY = 0;
	switch(volume->getType()) {
		case BOUNDING_BOX: {
			AABB* box = (AABB*)volume;
			searchX = box->getWidth();
			searchY = box->getHeight();
			break;
		}
			
		case BOUNDING_SPHERE: {
			SphereBound* sphere = (SphereBound*)volume;
			searchX = searchY = sphere->getRadius(); 
			break;
		}
	};
	
	// make sure we have a clean list
	result->clear();
	
	// put all spatials from the selected cells into result
	for(int i=hashY-searchY; i<hashY+searchY; i++) {
		for(int j=hashX-searchX; j<hashX+searchX; j++) {
			// check wether we're still inside the grid
			if(j > 0 && j < cellsX && 
			   i > 0 && i < cellsY) {
				CellPtr cell = cells[j][i];
				BOOST_FOREACH(SpatialPtr spatial, cell->spatials) {
					result->push_back(spatial);
				}
			}			
		}
	}
}
