#include <limits>

#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"
#include "NTTTManager.h"

static int playGame(NTTTGame& game, NTTTPlayer *players[], int player, int seed)
{
    srand(seed);

	players[0]->chooseOrder(game);
	players[1]->chooseOrder(game);

    while (true)
    {
        NTTTMove move(0, 0, 0);
        std::cout << game;
        move = players[player]->performMove(game);
        game.makeMove(move, NTTTBoard::RED);
        std::cout << "Player " << player+1 << " (" << players[player]->getName() << ") played: " << move << std::endl;
        player = 1 - player;

        if (!game.isActive())
        {
            std::cout << "Player " << player+1 << " (" << players[player]->getName() << ") won!" << std::endl;
            break;
        }
        std::cout << std::endl;
    }

    return player;
} // end of playGame

static void playMatch(int boardCount, int boardSize, int lineSize, int count)
{
    NTTTPlayerIce  playerIce;
    NTTTPlayerMike playerMike;
//    NTTTPlayer *players[] = {&playerIce, &playerMike};
    NTTTPlayer *players[] = {&playerMike, &playerIce};

    std::cout << "Playing match between " << players[0]->getName() << " and " << players[1]->getName() << std::endl;

    int wins[2] = {0, 0};
    int order[2] = {0, 0};
    for (int i=0; i<count; ++i)
    {
        int seed = i+101;
        int first = seed%2;
        std::cout << "Game " << i+1 << ", using seed: " << seed << std::endl;
        NTTTGame game;
        game.NewGame(boardCount, boardSize, lineSize); // Prepares the game with the chosen settings
        int winner = playGame(game, players, first, seed);
        std::cout << std::endl;
        wins[winner]++;
        if (first == winner)
            order[0]++;
        else
            order[1]++;
    }
    std::cout << "Player 1 (" << players[0]->getName() << ") wins: " << wins[0];
    std::cout << " (" << (wins[0]*100)/(wins[0]+wins[1]) << "%)" << std::endl;
    std::cout << "Player 2 (" << players[1]->getName() << ") wins: " << wins[1];
    std::cout << " (" << (wins[1]*100)/(wins[0]+wins[1]) << "%)" << std::endl;
    std::cout << "First player wins: " << order[0];
    std::cout << " (" << (order[0]*100)/(order[0]+order[1]) << "%)" << std::endl;
    std::cout << "Second player wins: " << order[1];
    std::cout << " (" << (order[1]*100)/(order[0]+order[1]) << "%)" << std::endl;
    exit(0);
} // end of playMatch

int main(int argc, char *argv[]) {

    //playMatch(4, 5, 3, 100);

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

