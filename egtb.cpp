/*
 * This is a feeble attempt to perform retrograde analysis on a single board.
 * So far it has been tested on:
 * *** 4x4 board, generating 64 kB of data.
 * *** 5x5 board, generating 32 MB of data. This takes about 3 seconds.
 *
 * Statistics:
 * 4x4 board:
 *  0: Wins =    1, Lost =    0, Dead =     0
 *  1: Wins =   12, Lost =    4, Dead =     0
 *  2: Wins =  110, Lost =   10, Dead =     0
 *  3: Wins =  392, Lost =  144, Dead =    24
 *  4: Wins = 1089, Lost =  429, Dead =   302
 *  5: Wins = 1888, Lost =  852, Dead =  1628
 *  6: Wins = 2236, Lost =  838, Dead =  4934
 *  7: Wins =  892, Lost = 1104, Dead =  9444
 *  8: Wins =  496, Lost =  159, Dead = 12215
 *  9: Wins =    0, Lost =   76, Dead = 11364
 * 10: Wins =    0, Lost =    0, Dead =  8008
 * 11: Wins =    0, Lost =    0, Dead =  4368
 * 12: Wins =    0, Lost =    0, Dead =  1820
 * 13: Wins =    0, Lost =    0, Dead =   560
 * 14: Wins =    0, Lost =    0, Dead =   120
 * 15: Wins =    0, Lost =    0, Dead =    16
 * 16: Wins =    0, Lost =    0, Dead =     1
 * Conclusion: The empty 4x4 board is a win, but only if you play in the corner.
 *
 * 5x5 board:
 *  0: Wins =      0, Lost =     1, Dead =       0
 *  1: Wins =     25, Lost =     0, Dead =       0
 *  2: Wins =    172, Lost =   128, Dead =       0
 *  3: Wins =   2128, Lost =   124, Dead =      48
 *  4: Wins =   8250, Lost =  3372, Dead =    1028
 *  5: Wins =  37478, Lost =  5498, Dead =   10154
 *  6: Wins =  86074, Lost = 29832, Dead =   61194
 *  7: Wins = 186676, Lost = 42250, Dead =  251774
 *  8: Wins = 236701, Lost = 92640, Dead =  752234
 *  9: Wins = 256387, Lost = 84249, Dead = 1702339
 * 10: Wins = 169158, Lost = 79160, Dead = 3020442
 * 11: Wins =  83384, Lost = 40818, Dead = 4333198
 * 12: Wins =  23580, Lost = 17577, Dead = 5159143
 * 13: Wins =   4717, Lost =  3904, Dead = 5191679
 * 14: Wins =    596, Lost =   452, Dead = 4456352
 * 15: Wins =     16, Lost =    44, Dead = 3268700
 * 16: Wins =      0, Lost =     1, Dead = 2042974
 * 17: Wins =      0, Lost =     0, Dead = 1081575
 * 18: Wins =      0, Lost =     0, Dead =  480700
 * 19: Wins =      0, Lost =     0, Dead =  177100
 * 20: Wins =      0, Lost =     0, Dead =   53130
 * 21: Wins =      0, Lost =     0, Dead =   12650
 * 22: Wins =      0, Lost =     0, Dead =    2300
 * 23: Wins =      0, Lost =     0, Dead =     300
 * 24: Wins =      0, Lost =     0, Dead =      25
 * 25: Wins =      0, Lost =     0, Dead =       1
 * Conclusion: The empty 5x5 board is a loss, no matter where you play.
 *
 */

/*
 * Uncomment the following line to generate a binary file with the data.
 * Currently, one byte is used for each position.
 *
 * Caveat: This currently only works on Linux
 */
#define USE_MMAP

/*
 * Uncomment the following line to dump all the solutions as ASCII.
 */
//#define DUMP_SOLUTIONS

/*
 * Ucomment the following line to dump all the statistics.
 */
#define DUMP_STATISTICS

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <assert.h>

#ifdef USE_MMAP
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

// Global variables
uint32_t g_boardSize;
uint32_t g_lineSize;
uint32_t g_numSquares;
uint32_t g_numPositions;

/*
 * This counts the number of '1' bits in the variable.
 * This corresponds to the number of X's on the board.
 */
int countBits(uint32_t pos)
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

static void setPos(uint32_t pos, uint8_t *array, int val)
{
    if (val)
        array[pos/8] |= (1 << (pos%8));
}

static uint32_t pow(uint32_t e)
{
    uint32_t res = 1;
    for (uint32_t i=0; i<e; ++i)
        res = res*2;

    return res;
}

/*
 * This prints out an ASCII representation of the board.
 */
static void printBoard(uint32_t pos)
{
    for (unsigned int i=0; i<g_numSquares; ++i)
    {
        uint32_t mask = 1ULL << i;
        if (pos & mask)
            std::cout << "1";
        else
            std::cout << "0";
    }
}; // end of printBoard

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

#ifdef USE_MMAP
    std::ostringstream oss;
    oss << "egtb-" << g_boardSize << "-" << g_lineSize << ".bit";
    std::string fileName = oss.str();

    std::cout << fileName << std::endl;

    int fd = open(fileName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // Read from existing file or create new.
    if (fd < 0)
    {
        perror("open");
        return (-1);
    }

    if (posix_fallocate(fd, 0, g_numPositions/8)) // Make sure new file has the right size
    {
        perror("fallocate");
        return (-1);
    }

    uint8_t *egtb = (uint8_t *) mmap(nullptr, g_numPositions/8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (egtb == MAP_FAILED) // Map the file to a pointer
    {
        perror("mmap");
        return (-1);
    }
#else
    uint8_t *egtb  = new uint8_t[g_numPositions/8];
    for (uint32_t pos=0; pos<g_numPositions/8; ++pos)
        egtb[pos] = 0;
#endif

    uint8_t *known = new uint8_t[g_numPositions/8];
    for (uint32_t pos=0; pos<g_numPositions/8; ++pos)
        known[pos] = 0;

    bool updated = true;
    // Repeat until no more updates.
    while (updated) 
    {
        std::cout << "." << std::flush;
        updated = false;
        // Loop through all positions
        for (uint32_t pos=0; pos<g_numPositions; ++pos)
        {
            if (readPos(pos, known))
                continue; // Skip if the position is already known.

            // This is a currently unknown position
            bool isKnown = true;
            bool win = true; // This is the value, if the board is dead.
            if (!lines.isBoardDead(pos))
            { 
                win = false; // Ok, the board is alive. Assume the worst

                // Loop through all legal moves
                for (unsigned int i=0; i<g_numSquares; ++i)
                {
                    uint32_t mask = 1ULL << i;
                    if (pos & mask) // Is square already occupied?
                        continue; // Then go to next square.
                    // Now we have a legal move

                    uint32_t new_pos = pos | mask;
                    if (!readPos(new_pos, known))
                    {
                        // If one child is unknown, then we can stop immediately.
                        isKnown = false;
                        break;
                    }
                    if (!readPos(new_pos, egtb))
                    {
                        // If one child is lost, then we are winning, and can stop immediately.
                        win = true;
                        break;
                    }
                    // If all children are won, then we are losing.
                } // Loop through all legal moves
            }

            if (isKnown)
            {
                setPos(pos, known, true);
                setPos(pos, egtb, win);
                updated = true;
            }
        } // Loop through all positions

    } // Repeat until no more updates.

    std::cout << std::endl;

#ifdef DUMP_SOLUTIONS
    for (uint32_t pos=0; pos<g_numPositions; ++pos)
    {
        if (!lines.isBoardDead(pos))
        {
            printBoard(pos);
            std::cout << " ";

            switch (readPos(pos, egtb))
            {
                case true  : std::cout << "1"; break;
                case false : std::cout << "0"; break;
            }
            std::cout << std::endl;
        }
    }
#endif

#ifdef DUMP_STATISTICS
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
#endif

#ifdef USE_MMAP
    munmap(egtb, g_numPositions/8);
    close(fd);
#endif

    std::cout << std::endl;

    return 0;
}

