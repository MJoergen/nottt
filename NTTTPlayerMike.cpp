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
int Board::makeBits(const NTTTGame& game)
{
    int numAlive = 0;
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
            std::cout << std::hex << std::setw(16) << std::setfill('0') << val << std::endl;
        m_bits.push_back(val);
        if (val)
            numAlive++;
    }
    if (m_debug)
        std::cout << "numAlive = " << numAlive << std::endl;
    return numAlive;
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
        return 99999;

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

    if (m_debug)
        std::cout << "alpha=" << std::dec << alpha << ", beta=" << beta << ", level=" << level << std::endl;

    int bestVal = -99999;

    int numActive = 0;
    for (int board=0; board<m_boardCount; ++board)
    {
        if (isBoardDead(m_bits[board]))
            continue;

        numActive++;
        for (int x=0; x<m_boardSize; ++x)
        {
            for (int y=0; y<m_boardSize; ++y)
            {
                int bit = x*8+y;
                uint64_t mask = 1UL << bit;
                if (m_bits[board] & mask)
                {
                    NTTTMove move(board, bit/8, bit%8);
                    if (m_debug)
                    {
                        for (int i=level; i<4; ++i)
                            std::cout << "  ";
                        std::cout << move << std::endl;
                    }
                    makeMove(board, bit);
                    int val = -alphaBeta(-beta, -alpha, level-1);
                    undoMove(board, bit);

                    if (m_debug)
                    {
                        for (int i=level; i<4; ++i)
                            std::cout << "  ";
                        std::cout << move << " -> " << std::dec << val << std::endl;
                    }

                    if (val > bestVal)
                    {
                        bestVal = val;
                    }

                    // Now comes the part specific for alpha-beta pruning:
                    // Since we are only interested, if another
                    // move is better, we update our lower bound.
                    if (val > alpha)
                    {
                        alpha = val;
                    }

                    // Now we check if the window has been closed.
                    // If so, then stop the search.
                    if (alpha >= beta)
                    {
                        // This is fail-soft, since we are returning the value best_val,
                        // which might be outside the window.
                        break;
                    }
                }
            }
        }
    }

    if (numActive == 0) // No legal moves
        return 99990 + level; // I've won

    return bestVal;
} // end of alphaBeta


/**
 * Return the best move.
 */
NTTTMove Board::findMove(int level)
{
    m_nodes = 0;
    int bestVal = -999999;
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
                    NTTTMove move(board, bit/8, bit%8);
                    if (m_debug)
                        std::cout << "Move " << move << std::endl;

                    makeMove(board, bit);
                    int val = -alphaBeta(-99999, 99999, level);
                    undoMove(board, bit);

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
    int numAlive = m_board.makeBits(game);
    return m_board.findMove(numAlive==1 ? 4 : (numAlive==2 ? 2 : 0));
} // end of performMove

