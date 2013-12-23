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

#include "config_world.h"

namespace mapcrafter {
namespace config {

WorldSection::WorldSection(bool global)
	: global(global) {
}

WorldSection::~WorldSection() {
}

void WorldSection::setGlobal(bool global) {
	this->global = global;
}

bool WorldSection::parse(const ConfigSection& section, const fs::path& config_dir,
		ValidationList& validation) {
	// set default configuration values
	world_name.setDefault(section.getName());

	mc::BlockPos crop_center;

	// go through all configuration options in this section
	//   - load/parse the individual options
	//   - warn the user about unknown options
	auto entries = section.getEntries();
	for (auto entry_it = entries.begin(); entry_it != entries.end(); ++entry_it) {
		std::string key = entry_it->first;
		std::string value = entry_it->second;

		if (key == "input_dir") {
			if (input_dir.load(key, value, validation)) {
				input_dir.setValue(BOOST_FS_ABSOLUTE(input_dir.getValue(), config_dir));
				if (!fs::is_directory(input_dir.getValue()))
					validation.push_back(ValidationMessage::error(
							"'input_dir' must be an existing directory! '"
							+ input_dir.getValue().string() + "' does not exist!"));
			}
		} else if (key == "world_name")
			world_name.load(key, value, validation);

		else if (key == "crop_min_y") {
			if (min_y.load(key, value, validation))
				worldcrop.setMinY(min_y.getValue());
		} else if (key == "crop_max_y") {
			if (max_y.load(key, value, validation))
				worldcrop.setMaxY(max_y.getValue());
		} else if (key == "crop_min_x") {
			if (min_x.load(key, value, validation))
				worldcrop.setMinX(min_x.getValue());
		} else if (key == "crop_max_x") {
			if (max_x.load(key, value, validation))
				worldcrop.setMaxX(max_x.getValue());
		} else if (key == "crop_min_z") {
			if (min_z.load(key, value, validation))
				worldcrop.setMinZ(min_z.getValue());
		} else if (key == "crop_max_z") {
			if (max_z.load(key, value, validation))
				worldcrop.setMaxZ(max_z.getValue());

		} else if (key == "crop_center_x")
			center_x.load(key, value, validation);
		else if (key == "crop_center_z")
			center_z.load(key, value, validation);
		else if (key == "crop_radius")
			radius.load(key, value, validation);

		else {
			validation.push_back(ValidationMessage::warning(
					"Unknown configuration option '" + key + "'!"));
		}
	}

	// validate the world croppping
	bool crop_rectangular = min_x.isLoaded() || max_x.isLoaded() || min_z.isLoaded() || max_z.isLoaded();
	bool crop_circular = center_x.isLoaded() || center_z.isLoaded() || radius.isLoaded();

	if (crop_rectangular && crop_circular) {
		validation.push_back(ValidationMessage::error(
				"You can not use both world cropping types at the same time!"));
	} else if (crop_rectangular) {
		if (min_x.isLoaded() && max_x.isLoaded() && min_x.getValue() > max_x.getValue())
			validation.push_back(ValidationMessage::error("min_x must be smaller than or equal to max_x!"));
		if (min_z.isLoaded() && max_z.isLoaded() && min_z.getValue() > max_z.getValue())
			validation.push_back(ValidationMessage::error("min_z must be smaller than or equal to max_z!"));
	} else if (crop_circular) {
		std::string message = "You have to specify crop_center_x, crop_center_z "
				"and crop_radius for circular world cropping!";
		center_x.require(validation, message)
			&& center_z.require(validation, message)
			&& radius.require(validation, message);

		worldcrop.setCenter(mc::BlockPos(center_x.getValue(), center_z.getValue(), 0));
		worldcrop.setRadius(radius.getValue());
	}

	if (min_y.isLoaded() && max_y.isLoaded() && min_y.getValue() > max_y.getValue())
		validation.push_back(ValidationMessage::error("min_y must be smaller than or equal to max_y!"));

	// check if required options were specified
	if (!global) {
		input_dir.require(validation, "You have to specify an input directory ('input_dir')!");
	}

	return isValidationValid(validation);
}

fs::path WorldSection::getInputDir() const {
	return input_dir.getValue();
}

std::string WorldSection::getWorldName() const {
	return world_name.getValue();
}

const mc::WorldCrop WorldSection::getWorldCrop() const {
	return worldcrop;
}

bool WorldSection::needsWorldCentering() const {
	// circular cropped worlds and cropped worlds with complete x- and z-bounds
	return (min_x.isLoaded() && max_x.isLoaded() && min_z.isLoaded() && max_z.isLoaded())
			|| center_x.isLoaded() || center_z.isLoaded() || radius.isLoaded();
}

} /* namespace config */
} /* namespace mapcrafter */
