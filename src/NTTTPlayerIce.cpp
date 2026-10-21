#include "NTTTPlayerIce.h"
#include "DataPath.h"
#include <fstream>
#include <sstream>

std::string NTTTPlayerIce::getName() {
    return "IceBot v2";
}

/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerIce::chooseOrder(const NTTTGame& game) {
    m_boardCount = game.getBoardCount();
    m_boardSize  = game.getBoardSize();
    m_lineSize   = game.getLineSize();
    loadEgtb();

    if (isBoardWon(game.getBoards()[0]))
        return FIRST;
    else
        return LAST;
} // end of chooseOrder

/**
 * Loads the endgame tablebase for the current board size and line size.
 * It is kept across games, and only reloaded when the sizes change.
 * Leaves m_egtb empty if there is no usable tablebase.
 */
void NTTTPlayerIce::loadEgtb() {
    if (m_boardSize == m_egtbBoardSize && m_lineSize == m_egtbLineSize)
        return;

    m_egtbBoardSize = m_boardSize;
    m_egtbLineSize  = m_lineSize;
    m_egtb.clear();

    // Tablebases can only be generated for these board sizes, see egtb.cpp.
    if (m_boardSize < 2 || m_boardSize > 5)
        return;

    std::ostringstream oss;
    oss << "tables/egtb-" << m_boardSize << "-" << m_lineSize << ".bit";
    std::string filePath = dataPath(oss.str());

    std::ifstream file(filePath, std::ifstream::binary);
    if (!file)
        return;

    file.seekg(0, file.end);
    std::streamoff fileSize = file.tellg();
    file.seekg(0, file.beg);

    // One bit per position. A file of any other size is not used.
    const std::streamoff expectedSize = (1LL << (m_boardSize * m_boardSize)) / 8;
    if (fileSize != expectedSize)
        return;

    m_egtb.resize(expectedSize);
    if (!file.read((char *)m_egtb.data(), expectedSize))
        m_egtb.clear();
} // end of loadEgtb

bool NTTTPlayerIce::isNumberEven(const int number) const {
    return (number % 2) == 0;
}

/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerIce::performMove(const NTTTGame& game) {
    unsigned int aliveBoards = 0;

    for (unsigned int boardNumber = 0; boardNumber < (unsigned int)m_boardCount; boardNumber++) {
        NTTTBoard board = game.getBoards()[boardNumber];
        if (board.getCurrentState() == NTTTBoard::ALIVE)
            aliveBoards++;
    }

    for (unsigned int boardNumber = 0; boardNumber < (unsigned int)m_boardCount; boardNumber++) {
        NTTTBoard board = game.getBoards()[boardNumber];
        if (board.getCurrentState() == NTTTBoard::DEAD)
            continue;
        if (isBoardWon(board))
            return playBestMove(boardNumber, board, aliveBoards);
    }
    unsigned int boardNumber = rand() % m_boardCount;
    while (game.getBoards()[boardNumber].getCurrentState() == NTTTBoard::DEAD) {
        boardNumber = rand() % m_boardCount;
    }

    return playBestMove(boardNumber, NTTTBoard(game.getBoards()[boardNumber]), aliveBoards);
} // end of performMove

bool NTTTPlayerIce::isBoardWon(const NTTTBoard board) const {

    bool isWon = false;

    if (!m_egtb.empty()) {
        // This numbers the squares row by row (bit y * size + x), while
        // egtb.cpp and MikeBot number them column by column (x * size + y).
        // That mirrors the board in its diagonal, which maps lines to lines,
        // so the position has the same value.
        unsigned int boardPosition = 0;
        for (unsigned int index = 0; index < (unsigned int)(m_boardSize * m_boardSize); index++) {
            int squareX = index % m_boardSize;
            int squareY = (int)(index / m_boardSize);
            if (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED) {
                boardPosition |= (1 << index);
            }
        }

        return (m_egtb[boardPosition / 8] >> (boardPosition % 8)) & 1;
    }

    // There is no usable tablebase, so guess.
    if (rand() % 2 == 0)
        isWon = true;

    return isWon;
}

NTTTMove NTTTPlayerIce::playBestMove(const int boardNumber, const NTTTBoard& board, const unsigned int otherBoards) const {
    unsigned int numOfNonKillingMoves = 0;
    bool shouldKill                   = false;
    if (isNumberEven(otherBoards)) {
        shouldKill = true;
    } else {
        shouldKill = false;
    }
    for (unsigned int index = 0; index < (unsigned int)(m_boardSize * m_boardSize); index++) {
        int squareX = index % m_boardSize;
        int squareY = (int)(index / m_boardSize);
        if (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED)
            continue;
        NTTTBoard tryBoard(board);
        bool survives = tryBoard.makeMove(squareX, squareY, NTTTBoard::BLUE);
        if (survives) {
            numOfNonKillingMoves++;
            if (shouldKill && isBoardWon(tryBoard))
                return NTTTMove(boardNumber, squareX, squareY);
            if (!shouldKill && !isBoardWon(tryBoard))
                return NTTTMove(boardNumber, squareX, squareY);
        } else if (shouldKill) {
            return NTTTMove(boardNumber, squareX, squareY);
        }
    }

    int squareX = rand() % m_boardSize;
    int squareY = rand() % m_boardSize;

    if (numOfNonKillingMoves > 0) {
        NTTTBoard tryBoard(board);
        while (true) {
            tryBoard = NTTTBoard(board);
            do {
                squareX = rand() % m_boardSize;
                squareY = rand() % m_boardSize;
            } while (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED);
            if (tryBoard.makeMove(squareX, squareY, NTTTBoard::BLUE))
                break;
        }
    } else {
        while (board.getSquareStates()[squareX][squareY] != NTTTBoard::UNMARKED) {
            squareX = rand() % m_boardSize;
            squareY = rand() % m_boardSize;
        }
    }

    return NTTTMove(boardNumber, squareX, squareY);
}
