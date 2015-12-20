#include <iomanip>

#include "NTTTPlayerMike.h"

/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerMike::chooseOrder(const NTTTGame& game)
{
    return UNDECIDED;
} // end of chooseOrder


/**
 * Converts a given line segment (start, direction, length)
 * into a 64-bit mask.
 */
static uint64_t makeLine(int x, int y, int dx, int dy, int size)
{
    uint64_t res = 0;
    for (int i=0; i<size; ++i)
    {
        res |= 1UL << (x*8+y);
        x += dx;
        y += dy;
    }
    return res;
} // end of makeLine


/**
 * Initializes the player with the new board configuration.
 */
void NTTTPlayerMike::NewGame(int boardCount, int boardSize, int lineSize)
{
    m_boardCount = boardCount;
    m_boardSize = boardSize;
    m_lineSize = lineSize;

    m_lines.clear();

    for (int x=0; x<boardSize; ++x)
    {
        for (int y=0; y<boardSize; ++y)
        {
            if (x+lineSize <= boardSize) // Horizontal
                m_lines.push_back(makeLine(x, y, 1, 0, lineSize));
            if (y+lineSize <= boardSize) // Vertical
                m_lines.push_back(makeLine(x, y, 0, 1, lineSize));
            if (x+lineSize <= boardSize && y+lineSize <= boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, 1, 1, lineSize));
            if (x >= lineSize-1 && y+lineSize <= boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, -1, 1, lineSize));
        }
    }
} // end of NewGame


/**
 * Generates a list of all legal moves in the current position.
 */
void NTTTPlayerMike::genMoves(const NTTTGame& game)
{
    m_moves.clear();

    const std::vector<NTTTBoard>& boards = game.getBoards();

    for (int boardNum = 0; boardNum < m_boardCount; ++boardNum)
    {
        const NTTTBoard& board = boards[boardNum];
        NTTTBoard::State state = board.getCurrentState();
        if (state != NTTTBoard::ALIVE)
            continue;

        const std::vector< std::vector<NTTTBoard::SquareState> >& squareStates = board.getSquareStates();
        for (int x = 0; x < m_boardSize; ++x)
        {
            for (int y = 0; y < m_boardSize; ++y)
            {
                if (squareStates[x][y] != NTTTBoard::UNMARKED)
                    continue;
                m_moves.push_back(NTTTMove(boardNum, x, y));
            }
        }
    }

    std::cout << m_moves.size() << " legal moves." << std::endl;

} // end of genMoves


/**
 * Calculate a bit mask of the current position.
 * @param game An instance of the NTTTGame.
 * @return The bit mask
 */
std::vector<uint64_t> NTTTPlayerMike::genMask(const NTTTGame& game)
{
    std::vector<uint64_t> res;

    const std::vector<NTTTBoard>& boards = game.getBoards();

    for (int boardNum = 0; boardNum < m_boardCount; ++boardNum)
    {
        const NTTTBoard& board = boards[boardNum];
        NTTTBoard::State state = board.getCurrentState();
        if (state != NTTTBoard::ALIVE)
        {
            res.push_back(0UL);
            continue;
        }

        uint64_t val = 0;
        const std::vector< std::vector<NTTTBoard::SquareState> >& squareStates = board.getSquareStates();
        for (int x = 0; x < m_boardSize; ++x)
        {
            for (int y = 0; y < m_boardSize; ++y)
            {
                if (squareStates[x][y] != NTTTBoard::UNMARKED)
                    continue;
                val |= 1UL << (x*8+y);
            }
        }
        std::cout << std::hex << std::setw(16) << std::setfill('0') << val << "  ";
        res.push_back(val);
    }
    std::cout << std::endl;

    return res;

} // end of genMask



/**
 * Calculate a static evaluation of the current position.
 * @param game An instance of the NTTTGame.
 * @return The evaluation.
 */
/*
int NTTTPlayerMike::evaluate(const NTTTGame& game)
{
    return 0;
} // end of evaluate
*/


/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerMike::performMove(const NTTTGame& game)
{
    genMoves(game);
    genMask(game);

    int boardCount = game.getBoardCount();
    int boardSize  = game.getBoardSize();

    while (true)
    {
        int boardNum = rand()%boardCount;
        const std::vector<NTTTBoard>& boards = game.getBoards();
        const NTTTBoard& board = boards[boardNum];
        NTTTBoard::State state = board.getCurrentState();
        if (state != NTTTBoard::ALIVE)
            continue;

        const std::vector< std::vector<NTTTBoard::SquareState> >& squareStates = board.getSquareStates();

        while (true)
        {
            int squareX = rand()%boardSize;
            int squareY = rand()%boardSize;

            if (squareStates[squareX][squareY] != NTTTBoard::UNMARKED)
                continue;

            return NTTTMove(boardNum, squareX, squareY);
        }
    }

    return NTTTMove(0, 0, 0); // Never used
} // end of performMove

