#include "NTTTPlayerIce.h"
#include <fstream>
#include <sstream>

std::string NTTTPlayerIce::getName() { return "IceBot v2"; }

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

#if !NEW_VERSION
	if (isNumberEven(m_lineSize * m_boardCount))
		return FIRST;
	else
	    return LAST;
#else
//	std::cout << "IsBoardWon: " << isBoardWon(game.getBoards()[0]) << std::endl;

	if (isBoardWon(game.getBoards()[0]))
		return FIRST;
	else
		return LAST;
#endif
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
#if !NEW_VERSION
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
				if (game.getBoards()[index].getSquareStates()[x][y] == NTTTBoard::UNMARKED)
                {
					if (tryMove(game, index, x, y))
						isAlmostDead = true;
					else{
						legalMoves++;
					}
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

#else
	unsigned int aliveBoards = 0;

	for (unsigned int index = 0; index < (unsigned int)m_boardCount; index++){
		NTTTBoard board = game.getBoards()[index];
		if (board.getCurrentState() == NTTTBoard::ALIVE)
			aliveBoards++;
	}

	for (unsigned int index = 0; index < (unsigned int) m_boardCount; index++){
		NTTTBoard board = game.getBoards()[index];
		if (board.getCurrentState() == NTTTBoard::DEAD)
			continue;
		if (isBoardWon(board))
			return playBestMove(index, board, aliveBoards);
	}
	unsigned int index = rand() % m_boardCount;
	while (game.getBoards()[index].getCurrentState() == NTTTBoard::DEAD){
		index = rand() % m_boardCount;
	}

	return playBestMove(index, NTTTBoard(game.getBoards()[index]), aliveBoards);

#endif

	// Unreachable code (at least when NEW_VERSION is 1)

	std::vector<NTTTMove> possibleMoves = getPossibleMoves(game);
	if (possibleMoves.size() > 0)
		return possibleMoves[rand() % possibleMoves.size()];

	return NTTTMove(0, 0, 0);
} // end of performMove

#if !NEW_VERSION
const bool NTTTPlayerIce::tryMove(const NTTTGame game, const int boardIndex, const int squareX, const int squareY) const {
	NTTTBoard board = game.getBoards()[boardIndex];
	
	return !board.makeMove(squareX, squareY, NTTTBoard::BLUE);
}
#else

const bool NTTTPlayerIce::isBoardWon(const NTTTBoard board) const{

	bool isWon = false;

	if (m_boardSize == 3 || m_boardSize == 4 || m_boardSize == 5){ //TODO: Change this to search for files
		unsigned int boardPosition = 0;
		for (unsigned int index = 0; index < (unsigned int)(m_boardSize * m_boardSize); index++){
			int squareX = index % m_boardSize;
			int squareY = (int)(index / m_boardSize);
			if (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED){
				boardPosition |= (1 << index);
			}
		}

        std::ostringstream oss;
        oss << "egtb-" << m_boardSize << "-" << m_lineSize << ".bit";
        std::string filePath = oss.str();

		std::ifstream inputFileStream(filePath, std::ifstream::binary);

        if (inputFileStream){
            inputFileStream.seekg(0, inputFileStream.end);
            unsigned int length = inputFileStream.tellg();
            if (length <= boardPosition/8){
                boardPosition = 0;
            }
            inputFileStream.seekg(boardPosition/8, inputFileStream.beg);

            char buffer;

            inputFileStream.read(&buffer, 1);
            inputFileStream.close();

            if (((buffer) >> (boardPosition%8)) & 1)
                isWon = true;
        }
	}
	else {
		if (rand() % 2 == 0)
			isWon = true;
	}

	return isWon;

	//TODO

}

const NTTTMove NTTTPlayerIce::playBestMove(const int boardNumber, const NTTTBoard& board, const unsigned int otherBoards) const {
	unsigned int numOfNonKillingMoves = 0;
	bool shouldKill = false;
	if (isNumberEven(otherBoards)){
		shouldKill = true;
	}
	else {
		shouldKill = false;
	}
	for (unsigned int index = 0; index < (unsigned int)(m_boardSize * m_boardSize); index++){
		int squareX = index % m_boardSize;
		int squareY = (int)(index / m_boardSize);
		if (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED)
			continue;
		NTTTBoard tryBoard(board);
		bool survives = tryBoard.makeMove(squareX, squareY, NTTTBoard::BLUE);
		if (survives){
			numOfNonKillingMoves++;
			if (shouldKill && isBoardWon(tryBoard))
				return NTTTMove(boardNumber, squareX, squareY);
			if (!shouldKill && !isBoardWon(tryBoard))
				return NTTTMove(boardNumber, squareX, squareY);
		}
		else if (shouldKill){
			return NTTTMove(boardNumber, squareX, squareY);
		}
	}

	int squareX = rand() % m_boardSize;
	int squareY = rand() % m_boardSize;
	
	if (numOfNonKillingMoves > 0){
		NTTTBoard tryBoard(board);
		while (true){
			tryBoard = NTTTBoard(board);
			do{
				squareX = rand() % m_boardSize;
				squareY = rand() % m_boardSize;
			}
			while (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED);
			if (tryBoard.makeMove(squareX, squareY, NTTTBoard::BLUE))
				break;
		}
	}
	else {
		while (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED){
			squareX = rand() % m_boardSize;
			squareY = rand() % m_boardSize;
		}
	}

	return NTTTMove(boardNumber, squareX, squareY);

}

#endif

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
