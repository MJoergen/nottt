#include <limits>

#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"
#include "NTTTManager.h"

int main(int argc, char *argv[]) {

	std::cout << "Program starts" << std::endl;

	if (!g_NtttManager.init()){ //Initializes SDL2, SDL2_image, SDL2_ttf and global variables. If something went wrong. The program is terminated.
		std::cout << "Press ENTER to continue..." << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return 1;
	}

	g_NtttManager.loop();
	g_NtttManager.close();

	std::cout << "Press ENTER to continue..." << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}

