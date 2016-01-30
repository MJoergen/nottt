#include <limits>
#include <iomanip>

#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"
#include "NTTTManager.h"

/*
 * Play a single game between two players.
 */
static unsigned int playGame(NTTTGame& game, std::vector<NTTTPlayer *> players, unsigned int first, unsigned int second, int seed)
{
    srand(seed);

    std::cout << "Playing game between " << players[first]->getName();
    std::cout << " and " << players[second]->getName();
    std::cout << " with seed " << seed << std::endl;

	players[first]->chooseOrder(game);
	players[second]->chooseOrder(game);

    unsigned int player = first;
    while (true)
    {
        NTTTMove move(0, 0, 0);
        std::cout << game;
        move = players[player]->performMove(game);
        std::cout << "Player " << players[player]->getName() << " played: " << move << std::endl;

        if (player == first)
            game.makeMove(move, NTTTBoard::RED);
        else
            game.makeMove(move, NTTTBoard::BLUE);

        player = first+second-player;

        if (!game.isActive())
        {
            std::cout << "Player " << players[player]->getName() << " won!" << std::endl;
            break;
        }
        std::cout << std::endl;
    }

    return player;
} // end of playGame

/*
 * Play an entire tournament (round robin) between an array of players.
 */
static void playMatch(std::vector<NTTTPlayer *> players, int boardCount, int boardSize, int lineSize, int count)
{
    // Contains the number of wins for each player against each other player.
    std::vector< std::vector<int> > table;

    for (unsigned int player=0; player<players.size(); ++player)
    {
        std::vector<int> wins(players.size(), 0);
        table.push_back(wins);
    }

    int order[2] = {0, 0}; // Separate statistics for whether the first or second player wins.

    // Play all the games
    for (int i=0; i<count; ++i)
    {
        int seed = i+1017;

        for (unsigned int diff = 1; diff<players.size(); diff++)
        {
            for (unsigned int first = 0; first < players.size(); first++)
            {
                int second = (first + diff) % players.size();

                std::cout << "Game " << i+1 << ":" << std::endl;

                NTTTGame game;
                game.NewGame(boardCount, boardSize, lineSize);
                unsigned int winner = playGame(game, players, first, second, seed);

                std::cout << std::endl;
                if (first == winner)
                {
                    table[first][second]++;
                    order[0]++; // First player won
                }
                else
                {
                    table[second][first]++;
                    order[1]++; // Second player won
                }
            }
        }
    }

    // Calculate total number of games. 
    int totalGames = players.size() * (players.size()-1) * count;
    int maxPoints = (players.size()-1) * count * 2;

    // Calculate the totals and sort them.
    std::vector< std::pair<int, int> > total;
    for (unsigned int player=0; player<players.size(); ++player)
    {
        int sum = 0;
        for (unsigned int opponent=0; opponent<players.size(); ++opponent)
        {
            sum += table[player][opponent];
        }
        total.push_back(std::make_pair(sum, player));
    }
    std::stable_sort(total.rbegin(), total.rend()); // Sort descending.

    // Display the results
    for (unsigned int i=0; i<players.size(); ++i)
    {
        int player = total[i].second;

        std::cout << "Player " << player+1;
        std::cout << " (" << std::setw(10) << players[player]->getName() << ") : ";
        for (unsigned int j=0; j<players.size(); ++j)
        {
            int opponent = total[j].second;

            if (player == opponent)
                std::cout << "XXX ";
            else
                std::cout << std::setw(3) << table[player][opponent] << " ";
        }
        int points = total[i].first;
        float win_percent = floor(points*100.0/maxPoints + 0.5);
        std::cout << " : " << std::setw(3) << points << " (" << win_percent << "%)" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "First player wins: " << order[0];
    std::cout << " (" << (order[0]*100)/totalGames << "%)" << std::endl;
    std::cout << "Second player wins: " << order[1];
    std::cout << " (" << (order[1]*100)/totalGames << "%)" << std::endl;
} // end of playMatch

/*
 * Main entry point
 */
int main(int argc, char *argv[]) {

/*
    NTTTPlayerIce  playerIce;
    NTTTPlayerMike playerMike1;
    playerMike1.setVersion(1);
    NTTTPlayerMike playerMike2;
    playerMike2.setVersion(2);
    NTTTPlayerMike playerMike3;
    playerMike3.setVersion(3);
    NTTTPlayerMike playerMike4;
    playerMike4.setVersion(4);
    std::vector<NTTTPlayer *> players = {&playerMike1, &playerMike2, &playerMike3, &playerMike4, &playerIce};

    playMatch(players, 2, 5, 3, 25);
    exit(0);
*/

	std::cout << "Program starts" << std::endl;

	NTTTManager ntttManager;

	if (!ntttManager.init()){ //Initializes SDL2, SDL2_image, SDL2_ttf and global variables. If something went wrong. The program is terminated.
		std::cout << "Press ENTER to continue..." << std::endl;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return 1;
	}

	ntttManager.loop();
	ntttManager.close();

	std::cout << "Press ENTER to continue..." << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return 0;
}

