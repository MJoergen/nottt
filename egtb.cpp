/*
 * This is a feeble attempt to perform retrograde analysis on a single board.
 * So far it has been tested on a 4x4 board, generating 64 kbit = 8 kbyte of data.
 * and on 5x5 board generating 32 megabit = 4 megabyte of data. This takes about 1 minute.
 *
 * 4x4 board
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
 *
 */

#include <iostream>
#include <vector>
#include <stdint.h>

#if 1
#define LINE_SIZE            3
#define BOARD_SIZE           4
#define NUM_SQUARES         16
#define NUM_POSITIONS   65536UL /* 2^16 */
#endif

#if 0
#define LINE_SIZE            3
#define BOARD_SIZE           5
#define NUM_SQUARES         25
#define NUM_POSITIONS 33554432UL /* 2^25 */
#endif

typedef enum 
{
    POS_UNKNOWN = 0,
    POS_WIN = 1,
    POS_LOST = 2
} pos_t;

std::vector<uint32_t> m_lines;

pos_t egtb[NUM_POSITIONS];

/**     
 * Converts a given line segment (start, direction, length)
 * into a 16-bit mask.
 */     
static uint32_t makeLine(int x, int y, int dx, int dy, int size)
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


static bool isBoardDead(uint32_t pos)
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

static void printBoard(uint32_t pos)
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

static int countBits(uint32_t pos)
{
    int count;
    for (count=0; pos; count++)
        pos &= pos-1;
    return count;
}; // end of countBits

static void init()
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

    std::cout << m_lines.size() << std::endl;
    for (unsigned int i=0; i<m_lines.size(); ++i)
    {
        printBoard(m_lines[i]);
        std::cout << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;

}; // end of init

int main()
{
    init();

    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
        egtb[pos] = POS_UNKNOWN;

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
            if (!isBoardDead(pos))
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

    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
    {
        if (!isBoardDead(pos))
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
            if (isBoardDead(pos))
                std::cout << "Dead";
            else
                std::cout << "Alive";
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

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
        if (!isBoardDead(pos))
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

    return 0;
}
