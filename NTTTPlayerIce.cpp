#include "NTTTPlayerIce.h"

std::string NTTTPlayerIce::getName() { return "IceBot v1"; }

/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerIce::chooseOrder(const NTTTGame& game)
{
	m_boardCount = game.getBoardCount();
	m_boardSize = game.getBoardSize();
	m_lineSize = game.getLineSize();

	if (isNumberEven(m_lineSize * m_boardCount))
		return FIRST;
	else
	    return LAST;
} // end of chooseOrder

const bool NTTTPlayerIce::isNumberEven(const int number) const{
    return (number%2) == 0;
}

/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerIce::performMove(const NTTTGame& game)
{
	std::vector<int> almostDeadBoards;
	std::vector<int> almostDeadLegalMoves;
	std::vector<int> aliveBoards;
	int sumOfLegalMoves = 0;
	int fewestLegalMovesIndex = 0;

	for (int index = 0; index < m_boardCount; index++){
		if (game.getBoards()[index].getCurrentState() == NTTTBoard::DEAD)
			continue;
		bool isAlmostDead = false;
		int legalMoves = 0;
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (tryMove(game, index, x, y))
					isAlmostDead = true;
				else{
					legalMoves++;
				}
			}
		}
		if (isAlmostDead){
			sumOfLegalMoves += legalMoves;
			almostDeadLegalMoves.push_back(legalMoves);
			almostDeadBoards.push_back(index);
			if (almostDeadBoards.size() > 0 && legalMoves + 1 < almostDeadLegalMoves[fewestLegalMovesIndex])
				fewestLegalMovesIndex = almostDeadBoards.size() - 1;
		}
		else {
			aliveBoards.push_back(index);
		}
	}

	int sumOfAlmostDeadBoards = almostDeadBoards.size();
	int sumOfAliveBoards = aliveBoards.size();

	if (isNumberEven(sumOfAlmostDeadBoards))
	{
		if (sumOfAlmostDeadBoards != 0){
			int almostDeadBoardIndex = rand() % sumOfAlmostDeadBoards;
			for (int index = 0; index < sumOfAlmostDeadBoards; index++){
				if (isNumberEven(almostDeadLegalMoves[index])){
					almostDeadBoardIndex = index;
					break;
				}
			}
			int boardIndex = almostDeadBoards[almostDeadBoardIndex];
			
			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (game.getBoards()[boardIndex].getSquareStates()[x][y] == NTTTBoard::UNMARKED)
						if (tryMove(game, boardIndex, x, y))
							return NTTTMove(boardIndex, x, y);
				}
			}
		}
	}
	else 
	{
		if (sumOfAlmostDeadBoards > 0 && almostDeadLegalMoves[fewestLegalMovesIndex] > 0){

			int boardIndex = almostDeadBoards[fewestLegalMovesIndex];

			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (game.getBoards()[boardIndex].getSquareStates()[x][y] == NTTTBoard::UNMARKED)
						if (!tryMove(game, boardIndex, x, y)){
							return NTTTMove(boardIndex, x, y);
						}
				}
			}

		}
		else if (sumOfAliveBoards > 0) {
			int aliveBoardIndex = rand() % sumOfAliveBoards;
			int boardIndex = aliveBoards[aliveBoardIndex];

			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (game.getBoards()[boardIndex].getSquareStates()[x][y] == NTTTBoard::UNMARKED)
						return NTTTMove(boardIndex, x, y);
				}
			}

		}
		else {
			int almostDeadBoardIndex = rand() % sumOfAlmostDeadBoards;
			for (int index = 0; index < sumOfAlmostDeadBoards; index++){
				if (!isNumberEven(almostDeadLegalMoves[index])){
					almostDeadBoardIndex = index;
					break;
				}
			}
			int boardIndex = almostDeadBoards[almostDeadBoardIndex];

			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (game.getBoards()[boardIndex].getSquareStates()[x][y] == NTTTBoard::UNMARKED)
						if (tryMove(game, boardIndex, x, y))
							return NTTTMove(boardIndex, x, y);
				}
			}

		}
	}

	std::vector<NTTTMove> possibleMoves = getPossibleMoves(game);
	if (possibleMoves.size() > 0)
		return possibleMoves[rand() % possibleMoves.size()];

	return NTTTMove(0, 0, 0);
} // end of performMove

const bool NTTTPlayerIce::tryMove(const NTTTGame game, const int boardIndex, const int squareX, const int squareY) const {
	NTTTBoard board = game.getBoards()[boardIndex];
	
	return !board.makeMove(squareX, squareY, NTTTBoard::BLUE);
}

std::vector<NTTTMove> NTTTPlayerIce::getPossibleMoves(const NTTTGame& game) const{
	std::vector<NTTTMove> possibleMoves;
	for (int index = 0; index < m_boardCount; index++){
		const NTTTBoard& board = game.getBoards()[index];
		if (board.getCurrentState() == NTTTBoard::DEAD)
			continue;
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					possibleMoves.push_back(NTTTMove(index, x, y));
				}
			}
		}
	}
	return possibleMoves;
}
