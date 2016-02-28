#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <assert.h>

// Global variables
uint32_t g_boardSize;
uint32_t g_lineSize;
uint32_t g_numSquares;
uint32_t g_numPositions;

/*
 * This counts the number of '1' bits in the variable.
 * This corresponds to the number of X's on the board.
 */
static int countBits(uint32_t pos)
{
    int count;
    for (count=0; pos; count++)
        pos &= pos-1;
    return count;
}; // end of countBits

/*
 * This is a helper class to quickly determine
 * whether the position contains a complete line.
 *
 * This currently only works up to 5x5 boards. To work on 6x6 boards all the
 * uint32_t variables must be changed to uint64_t.
 */
class Lines
{
    public:
        Lines(uint32_t boardSize, uint32_t lineSize) // Constructor
        {
            m_lines.clear();

            for (unsigned int x=0; x<boardSize; ++x)
            {
                for (unsigned int y=0; y<boardSize; ++y)
                {
                    if (x+lineSize <= boardSize) // Horizontal
                        m_lines.push_back(makeLine(boardSize, x, y, 1, 0, lineSize));
                    if (y+lineSize <= boardSize) // Vertical
                        m_lines.push_back(makeLine(boardSize, x, y, 0, 1, lineSize));
                    if (x+lineSize <= boardSize && y+lineSize <= boardSize) // Diagonal
                        m_lines.push_back(makeLine(boardSize, x, y, 1, 1, lineSize));
                    if (x >= lineSize-1 && y+lineSize <= boardSize) // Diagonal
                        m_lines.push_back(makeLine(boardSize, x, y, -1, 1, lineSize));
                }
            }
        }; // end of init

        /*
         * Returns true if the board contains a complete line
         */
        bool isBoardDead(uint32_t pos) const
        {
            for (uint32_t lineMask : m_lines)
            {
                if ((pos & lineMask) == lineMask)
                {
                    return true;
                }
            }
            return false;

        }; // end of isBoardDead

        void getLineCounts(uint32_t pos, uint32_t sq, int cnt[4]) const
        {
            for (uint32_t lineMask : m_lines)
            {
                if (!(sq & lineMask))
                    continue;
                int ix = countBits(pos & lineMask); // Will be between 0 and 3.
                cnt[ix]++;
            }
        }

        /*
        */
        void getStats(uint32_t pos, int sum[6]) const
        {
            sum[0] = 0;
            sum[1] = 0;
            sum[2] = 0;
            sum[3] = 0;
            sum[4] = 0;
            sum[5] = 0;

            // Loop through all legal moves
            for (unsigned int i=0; i<g_numSquares; ++i)
            {
                uint32_t mask = 1ULL << i;

                if (pos & mask) // Is square already occupied?
                    continue;

                int cnt[4] = {0, 0, 0, 0};
                getLineCounts(pos, mask, cnt);

                assert(cnt[3] == 0);
                if (cnt[2])
                    sum[5] ++;
                else if (cnt[1] == 4)
                    sum[4] ++;
                else if (cnt[1] == 3)
                    sum[3] ++;
                else if (cnt[1] == 2)
                    sum[2] ++;
                else if (cnt[1] == 1)
                    sum[1] ++;
                else if (cnt[0])
                    sum[0] ++;
            }
        } // end of getStats

    private:
        std::vector<uint32_t> m_lines;

        /**     
         * Private helper function. Used only in the constructor.
         *
         * Converts a given line segment (start, direction, length)
         * into a 16-bit mask.
         */     
        uint32_t makeLine(uint32_t boardSize, int x, int y, int dx, int dy, int size)
        {
            uint32_t res = 0;
            for (int i=0; i<size; ++i)
            {
                res |= 1ULL << (x*boardSize+y);
                x += dx;
                y += dy;
            }   
            return res;
        } // end of makeLine

}; // end of Lines

static int readPos(uint32_t pos, uint8_t *array)
{
    return (array[pos/8] >> (pos%8)) & 1;
}

static uint32_t pow(uint32_t e)
{
    uint32_t res = 1;
    for (uint32_t i=0; i<e; ++i)
        res = res*2;

    return res;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Wrong number of arguments" << std::endl;
        std::cout << "Usage: egtb <boardSize> <lineSize>" << std::endl;
        exit(-1);
    }

    g_boardSize = atol(argv[1]);
    g_lineSize  = atol(argv[2]);

    g_numSquares = g_boardSize * g_boardSize;
    g_numPositions = pow(g_numSquares);

    Lines lines(g_boardSize, g_lineSize);

    std::ostringstream oss;
    oss << "egtb-" << g_boardSize << "-" << g_lineSize << ".bit";
    std::string filePath = oss.str();

    std::ifstream file(filePath, std::ios::binary | std::ios::in);

    uint8_t *egtb = new uint8_t[g_numPositions/8];
    if (!file.read((char *) egtb, g_numPositions/8))
    {
        perror("read");
        delete [] egtb;
        exit(-1);
    }

    int numWins[g_numSquares+1];
    int numLost[g_numSquares+1];
    int numDead[g_numSquares+1];

    for (unsigned int i=0; i<=g_numSquares; ++i)
    {
        numWins[i] = 0;
        numLost[i] = 0;
        numDead[i] = 0;
    }

    int statWins[64];
    int statLost[64];

    for (int i=0; i<64; ++i)
    {
        statWins[i] = 0;
        statLost[i] = 0;
    }

    for (uint32_t pos=0; pos<g_numPositions; ++pos)
    {
        int numBits = countBits(pos);
        if (!lines.isBoardDead(pos))
        {
            int sum[6];
            lines.getStats(pos, sum);
            int par0 = sum[0] & 1;
            int par1 = sum[1] & 1;
            int par2 = sum[2] & 1;
            int par3 = sum[3] & 1;
            int par4 = sum[4] & 1;
            int par5 = sum[5] & 1;

            int index = (par0 << 5) + (par1 << 4) + (par2 << 3) + (par3 << 2) + (par4 << 1) + par5;

            if (readPos(pos, egtb))
            {
                numWins[numBits]++;
                statWins[index]++;
            }
            else
            {
                numLost[numBits]++;
                statLost[index]++;
            }
        }
        else
            numDead[numBits]++;
    }

    for (unsigned int i=0; i<=g_numSquares; ++i)
    {
        std::cout << std::setw(2) <<  i << ": ";
        std::cout << "Wins = " << std::setw(7) << numWins[i];
        std::cout << ", Lost = " << std::setw(7) << numLost[i];
        std::cout << ", Dead = " << std::setw(7) << numDead[i] << std::endl;
    }
    
    std::cout << "Index Wins Lost Score" << std::endl;
    for (int i=0; i<64; ++i)
    {
        std::cout << std::setw(2) << i << " " << statWins[i] << " " << std::setw(5) << statLost[i];
        int total = statWins[i] + statLost[i];
        int score = (statWins[i]*1000 + total/2)/total;
        std::cout << " " << score << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

