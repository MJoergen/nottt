#include <limits>

#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"
#include "NTTTManager.h"

static void test()
{
    NTTTGame       game;
	game.NewGame(3, 4, 3); // Prepares the game with the chosen settings

    NTTTPlayerIce  player1;
    NTTTPlayerMike player2;
	player1.chooseOrder(game);
	player2.chooseOrder(game);

    int player = 1;
    while (true)
    {
        NTTTMove move(0, 0, 0);
        std::cout << game;
        switch (player)
        {
            case 1:
                move = player1.performMove(game);
                game.makeMove(move, NTTTBoard::RED);
                break;
            case 2:
                move = player2.performMove(game);
                game.makeMove(move, NTTTBoard::BLUE);
                break;
        }
        std::cout << "Player " << player << " played: " << move << std::endl;
        player = 3 - player;

        if (!game.isActive())
        {
            std::cout << "Player " << player << " won!" << std::endl;
            break;
        }
        std::cout << std::endl;
    }
} // end of test

int main(int argc, char *argv[]) {

//    test();
//    exit(0);

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

