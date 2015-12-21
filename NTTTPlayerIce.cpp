#include "NTTTPlayerIce.h"

/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerIce::chooseOrder(const NTTTGame& game)
{
	boardCount = game.getBoardCount();
	boardSize = game.getBoardSize();
	lineSize = game.getLineSize();
    return UNDECIDED;
} // end of chooseOrder


/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerIce::performMove(const NTTTGame& game)
{
	std::vector<NTTTMove> possibleMoves = getPossibleMoves(game);
	if (possibleMoves.size() > 0)
		return possibleMoves[rand() % possibleMoves.size()];

	return NTTTMove(0, 0, 0);
} // end of performMove

std::vector<NTTTMove> NTTTPlayerIce::getPossibleMoves(const NTTTGame& game) const{
	std::vector<NTTTMove> possibleMoves;
	for (int index = 0; index < boardCount; index++){
		const NTTTBoard& board = game.getBoards()[index];
		for (int x = 0; x < boardSize; x++){
			for (int y = 0; y < boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					possibleMoves.push_back(NTTTMove(index, x, y));
				}
			}
		}
	}
	return possibleMoves;
}