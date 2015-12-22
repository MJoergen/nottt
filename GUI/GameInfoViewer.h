#ifndef _GAME_INFO_VIEWER_H_
#define _GAME_INFO_VIEWER_H_

#include <SDL.h>
#include "Text.h"
#include <vector>
#include "../NTTTMove.h"

class GameInfoViewer {

private:
	
public:
	
	GameInfoViewer();
	virtual ~GameInfoViewer();

	void renderGameInfoViewer() const;

	void init();
	void cleanUp();
	void addMove(NTTTMove move);
	void setWinner(int index); //Set the winner to player1 (index = 1) or player2 (index = 2)

};

#endif