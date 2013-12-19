#include <vector>

#include "../mc/world.h"
#include "../mc/worldhelper.h"

namespace mc = mapcrafter::mc;

int main(int argc, char** argv) {
	mc::World world;
	world.load(argv[1]);

	std::vector<mc::Sign> signs = mc::findSignsInWorld(world);
	for (auto it = signs.begin(); it != signs.end(); ++it) {
		std::cout << it->getPos() << ": " << it->getText() << std::endl;
	}
}
