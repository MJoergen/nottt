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
//#define USE_MMAP

/*
 * Uncomment the following line to dump all the solutions as ASCII.
 */
//#define DUMP_SOLUTIONS

/*
 * Ucomment the following line to dump all the statistics.
 */
//#define DUMP_STATISTICS

#include <iostream>
#include <vector>
#include <stdint.h>

#ifdef USE_MMAP
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#if 1
#define LINE_SIZE            3
#define BOARD_SIZE           4
#define NUM_SQUARES         16
#define NUM_POSITIONS   65536UL /* 2^16 */
#define FILE_NAME "egtb-4x4.dat"
#endif

#if 0
#define LINE_SIZE             3
#define BOARD_SIZE            5
#define NUM_SQUARES          25
#define NUM_POSITIONS  33554432UL /* 2^25 */
#define FILE_NAME "egtb-5x5.dat"
#endif

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
        Lines() // Constructor
        {
            m_lines.clear();

            for (int x=0; x<BOARD_SIZE; ++x)
            {
                for (int y=0; y<BOARD_SIZE; ++y)
                {
                    if (x+LINE_SIZE <= BOARD_SIZE) // Horizontal
                        m_lines.push_back(makeLine(x, y, 1, 0, LINE_SIZE));
                    if (y+LINE_SIZE <= BOARD_SIZE) // Vertical
                        m_lines.push_back(makeLine(x, y, 0, 1, LINE_SIZE));
                    if (x+LINE_SIZE <= BOARD_SIZE && y+LINE_SIZE <= BOARD_SIZE) // Diagonal
                        m_lines.push_back(makeLine(x, y, 1, 1, LINE_SIZE));
                    if (x >= LINE_SIZE-1 && y+LINE_SIZE <= BOARD_SIZE) // Diagonal
                        m_lines.push_back(makeLine(x, y, -1, 1, LINE_SIZE));
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

    private:
        std::vector<uint32_t> m_lines;

        /**     
         * Private helper function. Used only in the constructor.
         *
         * Converts a given line segment (start, direction, length)
         * into a 16-bit mask.
         */     
        uint32_t makeLine(int x, int y, int dx, int dy, int size)
        {
            uint32_t res = 0;
            for (int i=0; i<size; ++i)
            {
                res |= 1UL << (x*BOARD_SIZE+y);
                x += dx;
                y += dy;
            }   
            return res;
        } // end of makeLine

}; // end of Lines

/*
 * The " __attribute__ ((__packed__)) " part is to ensure, that
 * only one byte is used by the enum. Otherwise, it uses 4 bytes.
 */
typedef enum __attribute__ ((__packed__))
{
    POS_UNKNOWN = 0,
    POS_WIN = 1,
    POS_LOST = 2
} pos_t;

/*
 * This prints out an ASCII representation of the board.
 */
void printBoard(uint32_t pos)
{
    for (unsigned int i=0; i<NUM_SQUARES; ++i)
    {
        if (i%BOARD_SIZE == 0 && i>0)
            std::cout << std::endl;
        uint32_t mask = 1UL << i;
        if (pos & mask)
            std::cout << "x";
        else
            std::cout << ".";
    }
}; // end of printBoard

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

#ifndef USE_MMAP
pos_t egtb[NUM_POSITIONS];
#endif

int main()
{
    Lines lines;

#ifdef USE_MMAP
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // Read from existing file or create new.
    if (fd < 0)
    {
        perror("open");
        return (-1);
    }

    if (posix_fallocate(fd, 0, NUM_POSITIONS)) // Make sure new file has the right size
    {
        perror("fallocate");
        return (-1);
    }

    pos_t *egtb = (pos_t *) mmap(nullptr, NUM_POSITIONS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (egtb == MAP_FAILED) // Map the file to a pointer
    {
        perror("mmap");
        return (-1);
    }
#else
    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
        egtb[pos] = POS_UNKNOWN;
#endif

    bool updated = true;
    while (updated) 
    {
        std::cout << ".";
        updated = false;
        // Loop through all positions
        for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
        {
            if (egtb[pos] != POS_UNKNOWN)
                continue;
            // This is a currently unknown position

            pos_t thisVal = POS_WIN;
            if (!lines.isBoardDead(pos))
            {
                thisVal = POS_LOST; // Assume the worst
                // Loop through all legal moves
                for (int i=0; i<NUM_SQUARES; ++i)
                {
                    uint32_t mask = 1UL << i;
                    if (pos & mask)
                        continue;
                    // Now we have a legal move

                    uint32_t new_pos = pos | mask;
                    pos_t new_val = egtb[new_pos];
                    if (new_val == POS_UNKNOWN)
                    {
                        // If one child is unknown, then we can stop immediately.
                        thisVal = POS_UNKNOWN;
                        break;
                    }
                    if (new_val == POS_LOST)
                    {
                        // If one child is lost, then we are winning, and can stop immediately.
                        thisVal = POS_WIN;
                        break;
                    }
                }
            }

            egtb[pos] = thisVal;
            if (thisVal != POS_UNKNOWN)
                updated = true;
        }
    };
    std::cout << std::endl;
    std::cout << std::endl;

#ifdef DUMP_SOLUTIONS
    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
    {
        if (!lines.isBoardDead(pos))
        {
            printBoard(pos);
            std::cout << "Value = ";
            switch (egtb[pos])
            {
                case POS_WIN     : std::cout << "Win"; break;
                case POS_LOST    : std::cout << "Lost"; break;
                case POS_UNKNOWN : std::cout << "Unknown"; break;
            }
            std::cout << "  ";
            if (lines.isBoardDead(pos))
                std::cout << "Dead";
            else
                std::cout << "Alive";
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
#endif

#ifdef DUMP_STATISTICS
    int numWins[NUM_SQUARES+1];
    int numLost[NUM_SQUARES+1];
    int numDead[NUM_SQUARES+1];

    for (int i=0; i<=NUM_SQUARES; ++i)
    {
        numWins[i] = 0;
        numLost[i] = 0;
        numDead[i] = 0;
    }

    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
    {
        int numBits = countBits(pos);
        if (!lines.isBoardDead(pos))
        {
            if (egtb[pos] == POS_WIN)
                numWins[numBits]++;
            else if (egtb[pos] == POS_LOST)
                numLost[numBits]++;
        }
        else
            numDead[numBits]++;
    }

    for (int i=0; i<=NUM_SQUARES; ++i)
    {
        std::cout << i << ": ";
        std::cout << "Wins = " << numWins[i];
        std::cout << ", Lost = " << numLost[i];
        std::cout << ", Dead = " << numDead[i] << std::endl;
    }
#endif

#ifdef USE_MMAP
    munmap(egtb, NUM_POSITIONS);
    close(fd);
#endif

    return 0;
}

