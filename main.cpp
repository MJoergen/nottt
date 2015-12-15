#include "NTTTBoard.h"
#include "NTTTGame.h"
#include "NTTTMove.h"
#include "NTTTPlayer.h"
#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"

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
    NTTTGame game;
    game.NewGame(3, 4, 3);
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

int main()
{
    NTTTPlayerMike mike;
    NTTTPlayerIce ice;
    PlayGame(&mike, &ice);

    test(); // To be removed
    return 0;
}

