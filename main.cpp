#include "NTTTBoard.h"
#include "NTTTGame.h"
#include "NTTTMove.h"
#include "NTTTPlayer.h"
#include "NTTTPlayerMike.h"

int main()
{
    NTTTPlayerMike mike;
    NTTTGame game;
    game.NewGame(3, 4, 3);
    return 0;
}

