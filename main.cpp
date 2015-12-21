#include <limits>

#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"

extern bool init();
extern void loop();
extern void close();

int main(int argc, char *argv[]) {

	std::cout << "Program starts" << std::endl;

	if (!init()){ //Initializes SDL2, SDL2_image, SDL2_ttf and global variables. If something went wrong. The program is terminated.
		std::cout << "Press ENTER to continue..." << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return 1;
	}

	loop();
	close();

	std::cout << "Press ENTER to continue..." << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}

