#include <iomanip>
#include <assert.h>

#include "NTTTPlayerMike.h"

std::string NTTTPlayerMike::getName() { return "MikeBot"; }

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
 * Initializes data structures, particularly bit masks for lines.
 * Called once at beginning of each new game.
 */
void Board::init(const NTTTGame& game)
{
    m_boardCount = game.getBoardCount();
    m_boardSize  = game.getBoardSize();
    m_lineSize   = game.getLineSize();
    m_maxBits    = m_boardSize * m_boardSize;

    m_lines.clear();

    for (int x=0; x<m_boardSize; ++x)
    {
        for (int y=0; y<m_boardSize; ++y)
        {
            if (x+m_lineSize <= m_boardSize) // Horizontal
                m_lines.push_back(makeLine(x, y, 1, 0, m_lineSize));
            if (y+m_lineSize <= m_boardSize) // Vertical
                m_lines.push_back(makeLine(x, y, 0, 1, m_lineSize));
            if (x+m_lineSize <= m_boardSize && y+m_lineSize <= m_boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, 1, 1, m_lineSize));
            if (x >= m_lineSize-1 && y+m_lineSize <= m_boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, -1, 1, m_lineSize));
        }
    }
} // end of init


/**
 */
bool Board::isBoardDead(uint64_t bits) const
{
    for (uint64_t lineMask : m_lines)
    {
        if (!(bits & lineMask))
        {
            return true;
        }
    }
    return false;
} // end of isBoardDead


/**
 * Converts to internal bitmask representation
 * Called every time we are to make a move.
 */
void Board::makeBits(const NTTTGame& game)
{
    m_bits.clear();

    const std::vector<NTTTBoard>& boards = game.getBoards();

    for (int boardNum = 0; boardNum < m_boardCount; ++boardNum)
    {
        const NTTTBoard& board = boards[boardNum];
        NTTTBoard::State state = board.getCurrentState();
        if (state != NTTTBoard::ALIVE)
        {
            m_bits.push_back(0UL);
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

        if (isBoardDead(val))
            val = 0;

        if (m_debug)
            std::cout << std::hex << std::setw(16) << std::setfill('0') << val << " ";
        m_bits.push_back(val);
    }
//    std::cout << std::endl;
} // end of makeBits


/**
 * Make a move.
 */
void Board::makeMove(int board, int bit)
{
    uint64_t mask = 1UL << bit;
    assert (m_bits[board] & mask);
    m_bits[board] &= ~mask;

} // end of makeMove


/**
 * Undo a move.
 */
void Board::undoMove(int board, int bit)
{
    uint64_t mask = 1UL << bit;
    assert ((~m_bits[board]) & mask);
    m_bits[board] |= mask;
} // end of makeMove


/**
 * Return an estimate of the current position
 */
int Board::evaluate() const
{
    int numActive = 0;
    for (int board=0; board<m_boardCount; ++board)
    {
        if (!isBoardDead(m_bits[board]))
            numActive++;
    }

    if (numActive == 0)
        return 9999;

    return (rand() % 201) - 100;
} // end of evaluate


/**
 * Perform an alpha beta search
 * Returns a value
 */
int Board::alphaBeta(int alpha, int beta, int level)
{
    m_nodes++;
    if (level == 0)
    {
        return evaluate();
    }


    int bestVal = -999;

    for (int board=0; board<m_boardCount; ++board)
    {
        if (isBoardDead(m_bits[board]))
            continue;

        for (int x=0; x<m_boardSize; ++x)
        {
            for (int y=0; y<m_boardSize; ++y)
            {
                int bit = x*8+y;
                uint64_t mask = 1UL << bit;
                if (m_bits[board] & mask)
                {
                    makeMove(board, bit);
                    int val = -alphaBeta(-beta, -alpha, level-1);
                    undoMove(board, bit);

                    NTTTMove move(board, bit/8, bit%8);
                    if (m_debug)
                        std::cout << "  " << move << " -> " << val;

                    if (val > bestVal)
                    {
                        bestVal = val;
                    }
                }
            }
        }
    }

    if (bestVal == -999) // No legal moves
        return 999; // I've won

    return bestVal;
} // end of alphaBeta


/**
 * Return the best move.
 */
NTTTMove Board::findMove()
{
    m_nodes = 0;
    int bestVal = -99999;
    NTTTMove bestMove(0, 0, 0);

    for (int board=0; board<m_boardCount; ++board)
    {
        for (int x=0; x<m_boardSize; ++x)
        {
            for (int y=0; y<m_boardSize; ++y)
            {
                int bit = x*8+y;
                uint64_t mask = 1UL << bit;
                if (m_bits[board] & mask)
                {
                    makeMove(board, bit);
                    int val = -alphaBeta(-999, 999, 0);
                    undoMove(board, bit);

                    NTTTMove move(board, bit/8, bit%8);
                    if (m_debug)
                        std::cout << "Move " << move << " => " << std::dec << val << std::endl;

                    if (val > bestVal)
                    {
                        bestVal = val;
                        bestMove = move;
                    }
                }
            }
        }
    }
    if (m_debug)
    {
        std::cout << "nodes=" << m_nodes << std::endl;
        std::cout << "Best mode: " << bestMove << std::endl;
    }
    return bestMove;
} // end of findMove


/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerMike::chooseOrder(const NTTTGame& game)
{
    m_board.init(game);
    return UNDECIDED;
} // end of chooseOrder


/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerMike::performMove(const NTTTGame& game)
{
    if (m_debug)
        std::cout << game;
    m_board.makeBits(game);
    return m_board.findMove();
} // end of performMove

