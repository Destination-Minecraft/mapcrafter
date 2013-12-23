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

#ifndef CONFIGWORLD_H_
#define CONFIGWORLD_H_

#include "../mc/world.h"

#include "extended_ini.h"
#include "validation.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace mapcrafter {
namespace config {

class WorldSection {
private:
	bool global;

	Field<fs::path> input_dir;
	Field<std::string> world_name;

	Field<int> min_y, max_y;
	Field<int> min_x, max_x, min_z, max_z;
	Field<int> center_x, center_z, radius;
	mc::WorldCrop worldcrop;
public:
	WorldSection(bool global = false);
	~WorldSection();

	void setGlobal(bool global);
	bool parse(const ConfigSection& section, const fs::path& config_dir, ValidationList& validation);

	fs::path getInputDir() const;
	std::string getWorldName() const;
	const mc::WorldCrop getWorldCrop() const;
	bool needsWorldCentering() const;
};

} /* namespace config */
} /* namespace mapcrafter */

#endif /* CONFIGWORLD_H_ */
