#include <sstream>
#include <iomanip>
#include <assert.h>
#include <assert.h>
#include <fstream>
#include "trace.h"

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

/*
 * This counts the number of '1' bits in the variable.
 * This corresponds to the number of X's on the board.
 */
static int countBits(uint64_t pos)
{
    int count;
    for (count=0; pos; count++)
        pos &= pos-1;
    return count;
}; // end of countBits


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

    if (m_boardSize <= 5) {
        std::ostringstream oss;
        oss << "egtb-" << m_boardSize << "-" << m_lineSize << ".bit";
        std::string m_filePath = oss.str();

        std::ifstream file(m_filePath, std::ios::binary | std::ios::in);

        file.seekg (0, file.end);
        m_fileSize = file.tellg();
        file.seekg (0, file.beg);

        m_egtb = new uint8_t[m_fileSize];
        if (!file.read((char *) m_egtb, m_fileSize))
        {
            perror("read");
            delete [] m_egtb;
            m_egtb = nullptr;
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
 */
void Board::getLineCounts(uint64_t pos, uint64_t sq, int cnt[4]) const
{
    for (uint64_t lineMask : m_lines)
    {
        if (!(sq & lineMask))
            continue;
        int ix = countBits((~pos) & lineMask); // Will be between 0 and 3.
        cnt[ix]++;
    }
} // end of getLineCounts

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
            LOG(std::hex << std::setw(16) << std::setfill('0') << val << std::endl);
        m_bits.push_back(val);
        if (val)
            numAlive++;
    }
    if (m_debug)
        LOG("numAlive = " << numAlive << std::endl);
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

// These numbers are generated in egtb.cpp from the file egtb-5-3.bit
static const int values[64] = {
    625, 560, 725, 770, 742, 790, 747, 708,
    741, 783, 726, 702, 672, 637, 741, 760,
    704, 780, 773, 713, 750, 691, 714, 775,
    720, 656, 709, 774, 689, 793, 784, 733,
    737, 776, 780, 740, 741, 702, 720, 762,
    732, 705, 732, 772, 694, 769, 799, 762,
    777, 703, 716, 800, 727, 801, 796, 738,
    719, 787, 806, 723, 779, 710, 744, 789 };


static const int values_v6[1024] = {
545, 487, 701, 811, 654, 792, 857, 929,
732, 766, 857, 653, 822, 654, 400, 923,
754, 766, 811, 811, 821, 743, 778, 1000,
871, 760, 809, 906, 685, 902, 1000, 1000,
716, 740, 754, 744, 839, 732, 900, 909,
786, 721, 853, 867, 709, 870, 833, 0,
778, 747, 745, 773, 715, 801, 1000, 750,
743, 857, 786, 750, 868, 630, 500, 1000,
748, 780, 758, 789, 797, 786, 684, 955,
731, 675, 673, 913, 714, 914, 800, 250,
724, 688, 841, 878, 761, 842, 1000, 167,
686, 783, 889, 697, 889, 741, 1000, 1000,
714, 681, 769, 858, 754, 834, 818, 727,
704, 771, 895, 825, 890, 818, 0, 1000,
710, 732, 800, 712, 847, 734, 545, 750,
776, 770, 714, 905, 833, 875, 750, 250,
754, 778, 761, 836, 785, 679, 533, 1000,
713, 712, 797, 815, 654, 786, 1000, 400,
705, 694, 785, 828, 704, 810, 833, 778,
702, 799, 926, 641, 875, 618, 0, 1000,
696, 687, 723, 772, 695, 832, 1000, 333,
722, 783, 871, 667, 863, 643, 500, 800,
736, 753, 855, 891, 836, 685, 0, 1000,
783, 717, 773, 1000, 750, 925, 1000, 1000,
616, 584, 595, 752, 663, 767, 833, 700,
689, 752, 849, 691, 833, 676, 600, 1000,
699, 762, 795, 763, 853, 751, 833, 1000,
807, 732, 655, 897, 620, 944, 1000, 1000,
732, 727, 752, 763, 837, 733, 733, 929,
799, 768, 853, 892, 539, 910, 1000, 333,
794, 761, 833, 808, 737, 881, 750, 750,
749, 818, 917, 690, 738, 703, 0, 500,
720, 765, 711, 746, 846, 715, 833, 857,
747, 702, 432, 964, 609, 813, 1000, 500,
762, 724, 838, 786, 644, 841, 1000, 286,
681, 818, 900, 500, 722, 533, 0, 1000,
740, 706, 776, 762, 657, 843, 1000, 667,
718, 803, 944, 765, 868, 688, 250, 1000,
727, 779, 778, 720, 923, 689, 250, 1000,
886, 721, 818, 1000, 500, 720, 1000, 1000,
710, 678, 789, 805, 695, 826, 750, 750,
720, 784, 933, 722, 725, 712, 500, 1000,
731, 774, 848, 780, 842, 709, 500, 1000,
812, 695, 750, 800, 826, 880, 750, 0,
704, 737, 632, 757, 800, 718, 1000, 250,
843, 732, 737, 880, 600, 906, 1000, 667,
781, 737, 818, 760, 676, 865, 1000, 250,
752, 823, 1000, 800, 889, 778, 1000, 1000,
677, 614, 769, 676, 732, 822, 875, 900,
720, 786, 857, 667, 849, 644, 1000, 1000,
715, 754, 722, 700, 824, 682, 750, 1000,
831, 678, 692, 1000, 520, 938, 0, 0,
687, 753, 722, 797, 877, 696, 583, 1000,
828, 772, 722, 947, 674, 929, 500, 1000,
786, 729, 771, 724, 679, 871, 667, 800,
791, 794, 1000, 563, 793, 545, 0, 0,
705, 778, 737, 814, 745, 721, 143, 1000,
785, 672, 813, 810, 759, 909, 1000, 1000,
772, 730, 794, 964, 574, 805, 1000, 500,
683, 797, 778, 615, 941, 625, 0, 1000,
763, 736, 836, 662, 741, 817, 857, 571,
703, 809, 818, 824, 897, 650, 0, 1000,
774, 753, 742, 760, 868, 623, 333, 1000,
822, 709, 1000, 1000, 680, 1000, 0, 0,
712, 797, 526, 750, 879, 667, 750, 0,
796, 661, 1000, 1000, 727, 1000, 0, 0,
844, 686, 800, 500, 833, 1000, 1000, 0,
723, 894, 1000, 1000, 750, 333, 0, 0,
763, 737, 800, 724, 708, 679, 1000, 667,
671, 924, 1000, 1000, 857, 500, 0, 1000,
735, 876, 750, 750, 882, 706, 0, 0,
755, 705, 1000, 1000, 667, 600, 0, 0,
758, 691, 765, 667, 723, 788, 1000, 0,
685, 856, 600, 800, 923, 571, 0, 1000,
742, 840, 500, 625, 762, 667, 0, 0,
875, 677, 1000, 1000, 167, 1000, 0, 0,
724, 765, 500, 625, 813, 750, 0, 1000,
917, 709, 571, 0, 857, 765, 0, 0,
804, 707, 1000, 800, 889, 1000, 0, 0,
750, 816, 1000, 1000, 1000, 750, 0, 0,
764, 686, 850, 750, 805, 862, 1000, 1000,
684, 867, 750, 0, 1000, 750, 0, 0,
724, 809, 667, 833, 750, 875, 0, 1000,
867, 571, 500, 0, 1000, 1000, 0, 0,
714, 777, 286, 818, 775, 692, 0, 1000,
793, 672, 1000, 1000, 600, 938, 0, 1000,
804, 735, 1000, 667, 750, 889, 1000, 0,
718, 896, 0, 1000, 1000, 750, 0, 0,
673, 795, 267, 857, 893, 786, 1000, 1000,
898, 725, 1000, 1000, 750, 778, 0, 0,
804, 742, 1000, 1000, 750, 957, 0, 1000,
679, 844, 0, 0, 1000, 571, 0, 0,
828, 740, 889, 583, 813, 839, 1000, 0,
731, 870, 500, 0, 1000, 500, 0, 0,
752, 835, 1000, 1000, 667, 615, 1000, 0,
863, 816, 1000, 1000, 750, 1000, 0, 0,
808, 671, 727, 692, 639, 833, 1000, 1000,
667, 798, 1000, 571, 800, 750, 0, 0,
692, 813, 889, 667, 773, 636, 0, 0,
897, 688, 1000, 1000, 286, 889, 0, 0,
710, 820, 750, 769, 810, 658, 0, 1000,
800, 800, 500, 1000, 643, 750, 0, 0,
836, 685, 889, 600, 842, 941, 0, 0,
554, 929, 1000, 667, 1000, 1000, 0, 0,
712, 842, 600, 692, 875, 650, 1000, 1000,
855, 707, 1000, 1000, 429, 889, 0, 0,
842, 723, 714, 889, 739, 826, 0, 1000,
698, 900, 0, 0, 1000, 833, 0, 0,
801, 741, 833, 545, 806, 929, 0, 1000,
732, 883, 833, 250, 1000, 643, 0, 0,
674, 840, 1000, 800, 1000, 714, 0, 0,
811, 796, 0, 1000, 1000, 1000, 0, 0,
718, 822, 455, 857, 763, 912, 0, 1000,
783, 857, 500, 889, 500, 1000, 0, 1000,
811, 712, 667, 1000, 621, 917, 1000, 0,
741, 846, 1000, 500, 1000, 667, 1000, 0,
794, 687, 867, 556, 711, 861, 1000, 1000,
684, 903, 1000, 1000, 1000, 583, 0, 0,
766, 811, 0, 1000, 1000, 500, 1000, 0,
936, 563, 0, 0, 400, 1000, 0, 0,
787, 699, 615, 714, 677, 816, 1000, 1000,
617, 913, 1000, 714, 875, 462, 0, 0,
656, 816, 667, 0, 875, 636, 0, 1000,
860, 667, 0, 0, 250, 833, 0, 0,
719, 821, 706, 846, 721, 533, 1000, 1000,
821, 729, 500, 1000, 714, 1000, 0, 0,
845, 756, 833, 750, 733, 952, 0, 0,
744, 889, 1000, 500, 1000, 1000, 0, 0
};

void Board::initHash()
{
    for (int i=0; i<HASH_SIZE; ++i)
    {
        m_hashBoard[i] = 0;
        m_hashVal[i]   = 0;
    }

    m_statLookup    = 0;
    m_statFound     = 0;
    m_statWrote     = 0;
    m_statCollision = 0;
} // end of initHash

/**
 */
int Board::evaluateBoardV6(uint64_t board)
{
    uint64_t hash = board + (board >> 1) + (board >> 3) + (board >> 6) + (board >> 10) + (board >> 15);
    hash %= HASH_SIZE;

    m_statLookup ++;
    if (m_hashBoard[hash] == board)
    {
        m_statFound ++;
        return m_hashVal[hash];
    }

    int index = 0;

    // Loop through all legal moves
    for (int i=0; i<m_boardSize*m_boardSize; ++i)
    {
        uint64_t mask = 1ULL << i;

        if (!(board & mask)) // Is square already occupied?
            continue;
        int cnt[4] = {0, 0, 0, 0};
        getLineCounts(board, mask, cnt);

        assert(cnt[3] == 0);
        if (cnt[2])
            index ^= 0x001;
        else if (cnt[1] >= 8)
            index ^= 0x002;
        else if (cnt[1] == 7)
            index ^= 0x004;
        else if (cnt[1] == 6)
            index ^= 0x008;
        else if (cnt[1] == 5)
            index ^= 0x010;
        else if (cnt[1] == 4)
            index ^= 0x020;
        else if (cnt[1] == 3)
            index ^= 0x040;
        else if (cnt[1] == 2)
            index ^= 0x080;
        else if (cnt[1] == 1)
            index ^= 0x100;
        else if (cnt[0])
            index ^= 0x200;
    }

    int val = values_v6[index];

    if (m_hashBoard[hash])
    {
        m_statCollision ++;
    }
    else
    {
        m_statWrote ++;
    }

    m_hashVal[hash] = val;
    m_hashBoard[hash] = board;

    return val;

}; // end of evaluateBoardV6

/**
 * Return an estimate of the current position, relative to the player to move.
 */
int Board::evaluate() const
{
    int numActive  = 0;
    int almostDead = 0;
    int lastActive = 0;

    int sum = 0;

    if (m_version == 1)
        sum = (rand() % 201) - 100;

    for (int board=0; board<m_boardCount; ++board)
    {
        if (!isBoardDead(m_bits[board]))
        {
            lastActive = board;
            numActive++;

            if (m_version == 2)
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

            if (m_version >= 4)
            {
                // Count the number of boards that are almost dead
                bool kills = false;
                for (int x=0; x<m_boardSize && !kills; ++x)
                {
                    for (int y=0; y<m_boardSize && !kills; ++y)
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
                sum += almostDead*2000;
            }

            if (m_version == 5)
            {
                int sum0 = 0;
                int sum1 = 0;
                int sum2 = 0;
                int sum3 = 0;
                int sum4 = 0;
                int sum5 = 0;

                // Loop through all legal moves
                for (int i=0; i<m_boardSize*m_boardSize; ++i)
                {
                    uint64_t mask = 1ULL << i;

                    if (!(m_bits[board] & mask)) // Is square already occupied?
                        continue;
                    int cnt[4] = {0, 0, 0, 0};
                    getLineCounts(m_bits[board], mask, cnt);

                    assert(cnt[3] == 0);
                    if (cnt[2])
                        sum5 ++;
                    else if (cnt[1] == 4)
                        sum4 ++;
                    else if (cnt[1] == 3)
                        sum3 ++;
                    else if (cnt[1] == 2)
                        sum2 ++;
                    else if (cnt[1] == 1)
                        sum1 ++;
                    else if (cnt[0])
                        sum0 ++;
                }

                int par0 = sum0 & 1;
                int par1 = sum1 & 1;
                int par2 = sum2 & 1;
                int par3 = sum3 & 1;
                int par4 = sum4 & 1;
                int par5 = sum5 & 1;

                int index = (par0 << 5) + (par1 << 4) + (par2 << 3) + (par3 << 2) + (par4 << 1) + par5;
                sum += values[index];

            }

            if (m_version == 6)
            {
                sum += ((Board *)this)->evaluateBoardV6(m_bits[board]);
            }

        } // if (!isBoardDead(m_bits[board]))
    } // for (int board=0; board<m_boardCount; ++board)

    if (m_version >= 3) 
    {
        if (numActive == 1 && m_egtb != nullptr)
        {
            uint64_t bits = m_bits[lastActive];
            assert(!isBoardDead(bits));

            bits = ~bits & (m_fileSize*8-1);

            switch ((m_egtb[bits/8] >> (bits%8)) & 1)
            {
                case true : return 99990;
                case false: return -99990;
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
        LOG("alpha=" << std::dec << alpha << ", beta=" << beta << ", level=" << level << std::endl);

    int bestVal = -99990;

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
                            LOG("  ");
                        LOG(move << std::endl);
                    }
                    makeMove(board, mask);
                    int val = -alphaBeta(-beta, -alpha, level-1);
                    undoMove(board, mask);

                    if (m_debug)
                    {
                        NTTTMove move(board, x, y);
                        for (int i=level; i<4; ++i)
                            LOG("  ");
                        LOG(move << " -> " << std::dec << val << std::endl);
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
NTTTMove Board::findMove(const NTTTGame& game)
{
    if (m_debug)
        LOG(game);

    int numAlive = makeBits(game);

    // Determine search depth
    int level = 4;
    if (m_boardSize*numAlive > 4)
        level = 2;
    if (m_boardSize*numAlive > 6)
        level = 0;
    if (m_egtb && numAlive == 1)
       level = 0;

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
                        LOG("Move " << move << std::endl);

                    makeMove(board, mask);
                    int val = -alphaBeta(-99999, 99999, level);
                    undoMove(board, mask);

                    LOG("Move " << move << " => " << std::dec << val << std::endl);

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
    LOG("nodes=" << m_nodes << std::endl);
    LOG("m_statLookup=" << m_statLookup << std::endl);
    LOG("m_statFound=" << m_statFound << std::endl);
    LOG("m_statWrote=" << m_statWrote << std::endl);
    LOG("m_statCollision=" << m_statCollision << std::endl);
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
    return m_board.findMove(game);
} // end of performMove

