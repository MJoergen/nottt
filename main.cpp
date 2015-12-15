#include "NTTTBoard.h"
#include "NTTTGame.h"
#include "NTTTMove.h"
#include "NTTTPlayer.h"
#include "NTTTPlayerMike.h"
#include "NTTTPlayerIce.h"

int main()
{
    NTTTPlayerMike mike;
    NTTTPlayerIce ice;
    NTTTGame game;
    game.NewGame(3, 4, 3);
    return 0;
}

