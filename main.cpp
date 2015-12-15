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
    game.Dump();
    game.makeMove(NTTTMove(0, 0, 0), NTTTBoard::SquareState::RED);
    game.Dump();
}

int main()
{
    test(); // To be removed
    return 0;
}

