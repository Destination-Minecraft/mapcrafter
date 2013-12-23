/*
 * Copyright 2012, 2013 Moritz Hilscher
 *
 * This file is part of mapcrafter.
 *
 * mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../util.h"

#include "worldcrop.h"

#include <cmath>

namespace mapcrafter {
namespace mc {

WorldCrop::WorldCrop()
	: type(RECTANGULAR), radius(0) {
}

WorldCrop::~WorldCrop() {
}

void WorldCrop::setMinY(int value) {
	bounds_y.setMin(value);
}

void WorldCrop::setMaxY(int value) {
	bounds_y.setMax(value);
}

void WorldCrop::setMinX(int value) {
	bounds_x.setMin(value);
	bounds_chunk_x.setMin(util::floordiv(value, 16));
	bounds_region_x.setMin(util::floordiv(value, 512));

	type = RECTANGULAR;
}

void WorldCrop::setMaxX(int value) {
	bounds_x.setMax(value);
	bounds_chunk_x.setMax(util::floordiv(value, 16));
	bounds_region_x.setMax(util::floordiv(value, 512));

	type = RECTANGULAR;
}

void WorldCrop::setMinZ(int value) {
	bounds_z.setMin(value);
	bounds_chunk_z.setMin(util::floordiv(value, 16));
	bounds_region_z.setMin(util::floordiv(value, 512));

	type = RECTANGULAR;
}

void WorldCrop::setMaxZ(int value) {
	bounds_z.setMax(value);
	bounds_chunk_z.setMax(util::floordiv(value, 16));
	bounds_region_z.setMax(util::floordiv(value, 512));

	type = RECTANGULAR;
}

void WorldCrop::setCenter(const BlockPos& pos) {
	center = pos;
	type = CIRCULAR;
}

void WorldCrop::setRadius(int radius) {
	this->radius = radius;
	type = CIRCULAR;
}

bool WorldCrop::isRegionContained(const mc::RegionPos& region) const {
	if (type == RECTANGULAR) {
		// rectangular crop:
		// just check if the region is contained in the calculated bounds
		return bounds_region_x.contains(region.x) && bounds_region_z.contains(region.z);
	} else if (type == CIRCULAR) {
		// circular crop:
		// check roughly whether at least one block of the region is included
		// use the midpoint of the region and determine the distance to the center
		BlockPos region_center(region.x * 512 + 256, region.z * 512 + 256, 0);
		int dx = region_center.x - center.x;
		int dz = region_center.z - center.z;
		// and check whether it is at most radius + size of a region blocks away
		return (radius+512)*(radius+512) >= dx*dx + dz*dz;
	}

	return true;
}

bool WorldCrop::isChunkContained(const mc::ChunkPos& chunk) const {
	if (type == RECTANGULAR) {
		// rectangular crop:
		// just check if the chunk is contained in the calculated chunk bounds
		return bounds_chunk_x.contains(chunk.x) && bounds_chunk_z.contains(chunk.z);
	} else if (type == CIRCULAR) {
		// circular crop:
		// do the same thing doing with regions
		BlockPos chunk_center(chunk.x * 16 + 8, chunk.z * 16 + 8, 0);
		int dx = chunk_center.x - center.x;
		int dz = chunk_center.z - center.z;
		return (radius+16)*(radius+16) >= dx*dx + dz*dz;
	}

	return true;
}

bool WorldCrop::isChunkCompletelyContained(const mc::ChunkPos& chunk) const {
	mc::BlockPos corner(chunk.x * 16, chunk.z * 16, 0);
	return isBlockContainedXZ(corner)
			&& isBlockContainedXZ(corner + mc::BlockPos(15, 0, 0))
			&& isBlockContainedXZ(corner + mc::BlockPos(0, 15, 0))
			&& isBlockContainedXZ(corner + mc::BlockPos(15, 15, 0));
}

bool WorldCrop::isBlockContainedXZ(const mc::BlockPos& block) const {
	if (type == RECTANGULAR) {
		// rectangular crop:
		// just check if the chunk is contained in the bounds
		return bounds_x.contains(block.x) && bounds_z.contains(block.z);
	} else if (type == CIRCULAR) {
		// circular crop:
		// also check distance to the center (like with regions and chunks)
		int dx = block.x - center.x;
		int dz = block.z - center.z;
		return radius*radius >= dx*dx + dz*dz;
	}

	return true;
}

bool WorldCrop::isBlockContainedY(const mc::BlockPos& block) const {
	return bounds_y.contains(block.y);
}

}
}
