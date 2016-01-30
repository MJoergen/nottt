#include <sstream>
#include <iomanip>
#include <assert.h>
#include <assert.h>
#include <fstream>

#include "NTTTPlayerMike.h"

std::string NTTTPlayerMike::getName()
{ 
    std::ostringstream oss;
    oss << "MikeBot v" << m_board.getVersion();
    return oss.str();
}

/**
 * Converts a given line segment (start, direction, length)
 * into a 64-bit mask.
 */
static uint64_t makeLine(int x, int y, int dx, int dy, int size, int boardSize)
{
    uint64_t res = 0;
    for (int i=0; i<size; ++i)
    {
        res |= 1ULL << (x*boardSize+y);
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
    if (m_egtb)
    {
        delete [] m_egtb;
        m_egtb = nullptr;
    }

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
                m_lines.push_back(makeLine(x, y, 1, 0, m_lineSize, m_boardSize));
            if (y+m_lineSize <= m_boardSize) // Vertical
                m_lines.push_back(makeLine(x, y, 0, 1, m_lineSize, m_boardSize));
            if (x+m_lineSize <= m_boardSize && y+m_lineSize <= m_boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, 1, 1, m_lineSize, m_boardSize));
            if (x >= m_lineSize-1 && y+m_lineSize <= m_boardSize) // Diagonal
                m_lines.push_back(makeLine(x, y, -1, 1, m_lineSize, m_boardSize));
        }
    }

    if (m_boardSize == 4){
        m_filePath = "egtb-4x4.dat";
        m_fileSize = 65536; // 2^16
    }
    else if (m_boardSize == 5){
        m_filePath = "egtb-5x5.dat";
        m_fileSize = 33554432; // 2^25 = 32 MB 
    }

    std::ifstream file(m_filePath, std::ios::binary | std::ios::in);

    m_egtb = new val_t[m_fileSize];
    if (!file.read((char *) m_egtb, m_fileSize))
    {
        perror("read");
        delete [] m_egtb;
        m_egtb = nullptr;
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
                val |= 1ULL << (x*m_boardSize+y);
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
void Board::makeMove(int board, uint64_t mask)
{
    assert (m_bits[board] & mask);
    m_bits[board] &= ~mask;

} // end of makeMove


/**
 * Undo a move.
 */
void Board::undoMove(int board, uint64_t mask)
{
    assert ((~m_bits[board]) & mask);
    m_bits[board] |= mask;
} // end of makeMove


/**
 * Return an estimate of the current position, relative to the player to move.
 */
int Board::evaluate() const
{
    int numActive  = 0;
    int almostDead = 0;
    int lastActive = 0;

    int sum = 0;

    if (m_version==1)
        sum = (rand() % 201) - 100;

    for (int board=0; board<m_boardCount; ++board)
    {
        if (!isBoardDead(m_bits[board]))
        {
            lastActive = board;
            numActive++;

            if (m_version==2)
            {
                // Count the number of moves that don't kill the board
                int count = 0;
                for (int x=0; x<m_boardSize; ++x)
                {
                    for (int y=0; y<m_boardSize; ++y)
                    {
                        int bit = x*m_boardSize+y;
                        uint64_t mask = 1ULL << bit;
                        if (m_bits[board] & mask)
                        {
                            ((Board *)this)->makeMove(board, mask);
                            if (!isBoardDead(m_bits[board]))
                            {
                                count++;
                            }
                            ((Board *)this)->undoMove(board, mask);
                        }
                    }
                }
                sum += count % 2;
            }

            if (m_version==4)
            {
                // Count the number of boards that are almost dead
                bool kills = false;
                for (int x=0; x<m_boardSize and not kills; ++x)
                {
                    for (int y=0; y<m_boardSize and not kills; ++y)
                    {
                        int bit = x*m_boardSize+y;
                        uint64_t mask = 1ULL << bit;
                        if (m_bits[board] & mask)
                        {
                            ((Board *)this)->makeMove(board, mask);
                            kills = isBoardDead(m_bits[board]);
                            ((Board *)this)->undoMove(board, mask);
                            if (kills)
                            {
                                almostDead++;
                            }
                        }
                    }
                }
                sum += almostDead;
            }

        } // if (!isBoardDead(m_bits[board]))
    } // for (int board=0; board<m_boardCount; ++board)

    if (m_version >= 3) 
    {
        if (numActive == 1 and m_egtb != nullptr)
        {
            uint64_t bits = m_bits[lastActive];
            assert(!isBoardDead(bits));

            bits = ~bits & (m_fileSize-1);

            switch (m_egtb[bits])
            {
                case POS_WIN:  return 99999;
                case POS_LOST: return -99999;
                default: assert(false);
            }
        }
    }

    if (numActive == 0)
        return 99999;

    return sum;
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
                int bit = x*m_boardSize+y;
                uint64_t mask = 1ULL << bit;
                if (m_bits[board] & mask)
                {
                    if (m_debug)
                    {
                        NTTTMove move(board, x, y);
                        for (int i=level; i<4; ++i)
                            std::cout << "  ";
                        std::cout << move << std::endl;
                    }
                    makeMove(board, mask);
                    int val = -alphaBeta(-beta, -alpha, level-1);
                    undoMove(board, mask);

                    if (m_debug)
                    {
                        NTTTMove move(board, x, y);
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
    std::vector<NTTTMove> bestMoves;

    for (int board=0; board<m_boardCount; ++board)
    {
        for (int x=0; x<m_boardSize; ++x)
        {
            for (int y=0; y<m_boardSize; ++y)
            {
                int bit = x*m_boardSize+y;
                uint64_t mask = 1ULL << bit;
                if (m_bits[board] & mask)
                {
                    NTTTMove move(board, x, y);
                    if (m_debug)
                        std::cout << "Move " << move << std::endl;

                    makeMove(board, mask);
                    int val = -alphaBeta(-99999, 99999, level);
                    undoMove(board, mask);

                    if (m_debug)
                        std::cout << "Move " << move << " => " << std::dec << val << std::endl;

                    if (val > bestVal)
                    {
                        bestVal = val;
                        bestMoves.clear();
                    }
                    if (val >= bestVal)
                    {
                        bestMoves.push_back(move);
                    }
                }
            }
        }
    }
    assert(bestVal > -999999);
    assert(bestMoves.size() > 0);
    NTTTMove bestMove = bestMoves[rand() % bestMoves.size()];
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
    switch (rand()%2)
    {
        case 0 : return FIRST;
        case 1 : return LAST;
    }
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
    return m_board.findMove(numAlive==1 ? 0 : (numAlive==2 ? 2 : 0));
} // end of performMove

