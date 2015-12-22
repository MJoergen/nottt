#include "NTTTPlayerIce.h"

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

	for (int index = 0; index < m_boardCount; index++){
		if (m_lineSize != 1)
			m_aliveBoards.push_back(index);
		else{
			m_almostDeadBoards.push_back(index);
			m_almostDeadBoardsLegalMoves.push_back(0);
		}
	}

	if (!isNumberEven(m_lineSize * m_boardCount))
		return FIRST;
	else
	    return LAST;
} // end of chooseOrder

const bool NTTTPlayerIce::isNumberEven(const int number) const{
    return (number%2) == 0;
}

NTTTMove NTTTPlayerIce::playAliveMove(const NTTTGame game) const{
	NTTTBoard board = game.getBoards()[m_aliveBoards[rand() % m_aliveBoards.size()]];
	std::vector<NTTTMove> moves;

	for (int x = 0; x < m_boardSize; x++){
		for (int y = 0; y < m_boardSize; y++){
			if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
				moves.push_back(NTTTMove(m_aliveBoards[0], x, y));
			}
		}
	}

	return moves[rand() % moves.size()];
}

NTTTMove NTTTPlayerIce::playAlmostDeadWithOddLegalMoves(const NTTTGame game) const{
	for (unsigned int index = 0; index < m_almostDeadBoardsLegalMoves.size(); index++){
		if (!isNumberEven(m_almostDeadBoardsLegalMoves[index])){

			NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];

			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
						if (!board.makeMove(x, y, NTTTBoard::BLUE)){
							return NTTTMove(m_almostDeadBoards[index], x, y);
						}
						else{
							board.makeMove(x, y, NTTTBoard::UNMARKED);
						}
					}
				}
			}

		}
	}

	for (unsigned int index = 0; index < m_almostDeadBoards.size(); index++){
		NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					if (!board.makeMove(x, y, NTTTBoard::BLUE)){
						return NTTTMove(m_almostDeadBoards[index], x, y);
					}
					else{
						board.makeMove(x, y, NTTTBoard::UNMARKED);
					}
				}
			}
		}
	}

	return NTTTMove(0, 0, 0);
}

NTTTMove NTTTPlayerIce::playAlmostDeadWithEvenLegalMoves(const NTTTGame game) const{
	for (unsigned int index = 0; index < m_almostDeadBoardsLegalMoves.size(); index++){
		if (isNumberEven(m_almostDeadBoardsLegalMoves[index])){
			
			NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];

			for (int x = 0; x < m_boardSize; x++){
				for (int y = 0; y < m_boardSize; y++){
					if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
						if (!board.makeMove(x, y, NTTTBoard::BLUE)){
							return NTTTMove(m_almostDeadBoards[index], x, y);
						}
						else{
							board.makeMove(x, y, NTTTBoard::UNMARKED);
						}
					}
				}
			}

		}
	}

	for (unsigned int index = 0; index < m_almostDeadBoards.size(); index++){
		NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					if (!board.makeMove(x, y, NTTTBoard::BLUE)){
						return NTTTMove(m_almostDeadBoards[index], x, y);
					}
					else{
						board.makeMove(x, y, NTTTBoard::UNMARKED);
					}
				}
			}
		}
	}
	
	return NTTTMove(0, 0, 0);

}

NTTTMove NTTTPlayerIce::playAlmostDeadLegalMove(const NTTTGame game) const{
	for (unsigned int index = 0; index < m_almostDeadBoards.size(); index++){
		NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					if (board.makeMove(x, y, NTTTBoard::BLUE)){
						return NTTTMove(m_almostDeadBoards[index], x, y);
					}
					else{
						board.makeMove(x, y, NTTTBoard::UNMARKED);
					}
				}
			}
		}
	}
	
	for (unsigned int index = 0; index < m_almostDeadBoards.size(); index++){
		NTTTBoard board = game.getBoards()[m_almostDeadBoards[index]];
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					if (!board.makeMove(x, y, NTTTBoard::BLUE)){
						return NTTTMove(m_almostDeadBoards[index], x, y);
					}
					else{
						board.makeMove(x, y, NTTTBoard::UNMARKED);
					}
				}
			}
		}
	}

	return NTTTMove(0, 0, 0);
}

/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerIce::performMove(const NTTTGame& game)
{
	for (unsigned int index = 0; index < m_aliveBoards.size(); index++){
		NTTTBoard board = game.getBoards()[m_aliveBoards[index]];
		bool shouldBreak = false;
		for (int x = 0; x < m_boardSize; x++){
			for (int y = 0; y < m_boardSize; y++){
				if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
					if (!board.makeMove(x, y, NTTTBoard::BLUE)){
						transferAliveBoard(game, m_aliveBoards[index]);
						m_aliveBoards.erase(m_aliveBoards.begin() + index);
						shouldBreak = true;
						break;
					}
					else
						board.makeMove(x, y, NTTTBoard::UNMARKED);
				}
			}
			if (shouldBreak) break;
		}
		if (shouldBreak) continue;
	}

	int sumOfLegalMoves = 0;

	for (unsigned int index = 0; index < m_almostDeadBoards.size(); index++){
		if (game.getBoards()[m_almostDeadBoards[index]].getCurrentState() == NTTTBoard::DEAD){
			m_almostDeadBoards.erase(m_almostDeadBoards.begin() + index);
			m_almostDeadBoardsLegalMoves.erase(m_almostDeadBoardsLegalMoves.begin() + index);
		}
		else {
			sumOfLegalMoves += m_almostDeadBoardsLegalMoves[index];
		}
	}

	if (isNumberEven(m_almostDeadBoards.size())){
		if (isNumberEven(sumOfLegalMoves)){
			if (m_aliveBoards.size() != 0){
				return playAliveMove(game);
			}
			else {
				return playAlmostDeadWithOddLegalMoves(game);
			}
		}
		else {
			return playAlmostDeadLegalMove(game);
		}
	}
	else{
		if (isNumberEven(sumOfLegalMoves) && m_almostDeadBoards.size() != 1)
				return playAlmostDeadWithEvenLegalMoves(game);
		else{
			if (m_aliveBoards.size() != 0)
				return playAliveMove(game);
			else
				return playAlmostDeadLegalMove(game);
		}
	}
		


	std::vector<NTTTMove> possibleMoves = getPossibleMoves(game);
	if (possibleMoves.size() > 0)
		return possibleMoves[rand() % possibleMoves.size()];

	return NTTTMove(0, 0, 0);
} // end of performMove

void NTTTPlayerIce::transferAliveBoard(const NTTTGame game, const int boardIndex) {
	m_almostDeadBoards.push_back(boardIndex);
	NTTTBoard board = game.getBoards()[boardIndex];
	int legalMoves = 0;
	for (int x = 0; x < m_boardSize; x++){
		for (int y = 0; y < m_boardSize; y++){
			if (board.getSquareStates()[x][y] == NTTTBoard::UNMARKED){
				if (board.makeMove(x, y, NTTTBoard::BLUE))
					legalMoves++;
				board.makeMove(x, y, NTTTBoard::UNMARKED);
			}
		}
	}
	m_almostDeadBoardsLegalMoves.push_back(legalMoves);
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
