#include <limits>

#include "NTTTBoard.h"
#include "NTTTGame.h"
#include "NTTTMove.h"
#include "NTTTPlayer.h"
#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"

extern bool init();
extern void loop();
extern void close();

static void test()
{
    NTTTPlayerMike mike;
    NTTTPlayerIce ice;
    NTTTGame game;
    game.NewGame(3, 4, 3);
    std::cout << game;
    game.makeMove(NTTTMove(0, 0, 0), NTTTBoard::SquareState::RED);
    std::cout << game;
}

int PlayGame(NTTTPlayer *player1, NTTTPlayer *player2)
{
    unsigned int boardCount = 3;
    unsigned int boardSize = 4;
    unsigned int lineSize = 3;

    NTTTGame game;
    game.NewGame(boardCount, boardSize, lineSize);
    player1->NewGame(boardCount, boardSize, lineSize);
    player2->NewGame(boardCount, boardSize, lineSize);
    std::cout << game;

    int player = 1;
    NTTTMove move(0, 0, 0);
    for (int i=0; i<10; ++i)
    {
        switch (player)
        {
            case 1 : 
                move = player1->performMove(game);
                game.makeMove(move, NTTTBoard::RED);
                break;
            case 2 : 
                move = player2->performMove(game);
                game.makeMove(move, NTTTBoard::BLUE);
                break;
        }
        std::cout << "Player " << player << " moved: " << move << std::endl;
        player = 3 - player;
    }
    
    std::cout << game;
    return 0; // The winner.
} // end of PlayGame


/*

Dependencies: SDL2, SDL2_image, SDL2_ttf

*/


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


/*
int main()
{
    NTTTPlayerMike mike;
    NTTTPlayerIce ice;
    PlayGame(&mike, &ice);

    test(); // To be removed
    return 0;
}
*/
