#ifndef _NTTTPLAYERICE_H_
#define _NTTTPLAYERICE_H_

#include "NTTTPlayer.h"

/**
 * This is an abstract base class that is to be derived from.
 * To implement a player, you should write something like:
 *
 * #include "NTTTPlayer.h"
 * class HTTPPlayerIce : HTTPPlayer 
 * {
 * }
 *
 */

class NTTTPlayerIce : public NTTTPlayer {
private:
	int m_boardCount, m_boardSize, m_lineSize;
	std::vector<int> m_almostDeadBoards, m_aliveBoards;
	std::vector<int> m_almostDeadBoardsLegalMoves;
	std::vector<NTTTMove> getPossibleMoves(const NTTTGame& game) const;
	void transferAliveBoard(const NTTTGame game, const int boardIndex);
	const bool isNumberEven(const int number) const;
	NTTTMove playAliveMove(const NTTTGame game) const;
	NTTTMove playAlmostDeadWithEvenLegalMoves(const NTTTGame game) const;
	NTTTMove playAlmostDeadWithOddLegalMoves(const NTTTGame game) const;
	NTTTMove playAlmostDeadLegalMove(const NTTTGame game) const;
public:
	/**
	 * Run in the initialization phase of the game.
	 * @param game An instance of the NTTTGame.
	 * @return The player's/bot's choice of order.
	 */
	virtual OrderChoice chooseOrder(const NTTTGame& game);

	/**
	 * Run continually as the game progresses.
	 * @param game An instance of the NTTTGame.
	 * @return The player's/bot's move.
	 */
	virtual NTTTMove performMove(const NTTTGame& game);

	/**
	 * The destructor must always be made virtual
	 */
	virtual ~NTTTPlayerIce() {}
}; // end of class NTTTPlayerIce

#endif // _NTTTPLAYERICE_H_

