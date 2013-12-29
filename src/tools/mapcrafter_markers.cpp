#include <vector>

#include "../config/config.h"
#include "../mc/world.h"
#include "../mc/worldhelper.h"

namespace config = mapcrafter::config;
namespace mc = mapcrafter::mc;

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Usage: ./mapcrafter_markers [configfile]" << std::endl;
		return 1;
	}

	std::string configfile = argv[1];

	config::MapcrafterConfigFile parser;
	config::ValidationMap validation;
	bool ok = parser.parse(configfile, validation);

	if (validation.size() > 0) {
		std::cerr << (ok ? "Some notes on your configuration file:" : "Your configuration file is invalid!") << std::endl;
		for (auto it = validation.begin(); it != validation.end(); ++it) {
			std::cerr << it->first << ":" << std::endl;
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				std::cerr << " - " << *it2 << std::endl;
			}
		}
	} else {
		std::cerr << "Everything ok." << std::endl;
	}

	auto worlds = parser.getWorlds();
	auto markers = parser.getMarkers();
	for (auto world_it = worlds.begin(); world_it != worlds.end(); ++world_it) {
		mc::WorldCrop worldcrop = world_it->second.getWorldCrop();
		mc::World world;
		world.setWorldCrop(worldcrop);
		if (!world.load(world_it->second.getInputDir().string())) {
			std::cerr << "Error: Unable to load world " << world_it->first << "!" << std::endl;
			continue;
		}

		std::vector<mc::Sign> signs = mc::findSignsInWorld(world);
		for (auto sign_it = signs.begin(); sign_it != signs.end(); ++sign_it) {
			// don't use signs not contained in the world boundaries
			if (!worldcrop.isBlockContainedXZ(sign_it->getPos())
					&& !worldcrop.isBlockContainedY(sign_it->getPos()))
				continue;
			for (auto marker_it = markers.begin(); marker_it != markers.end(); ++marker_it) {
				if (marker_it->second.matchesSign(*sign_it))
					std::cout << marker_it->second.formatText(*sign_it) << std::endl;
			}
		}
	}
}
