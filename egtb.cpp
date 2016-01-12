/*
 * This is a feeble attempt to perform retrograde analysis on a single board.
 * So far it has been tested on a 4x4 board, generating 64 kbit = 8 kbyte of data.
 * A 5x5 board will lead to 32 megabit = 4 megabyte of data. This has not been tested yet,
 * but the data types need to be changed from uint16_t to uint32_t.
 *
 * This version is very fast, it takes only a fraction of a second, and gives the following result:
 * Out of all the 2^16 possible positions:
 * Wins = 7116
 * Lost = 3616
 * Dead = 54804 (three or more in a row).
 *
 */

#include <iostream>
#include <vector>
#include <stdint.h>

#define LINE_SIZE         3
#define BOARD_SIZE        4
#define NUM_SQUARES      16
#define NUM_POSITIONS 65536 /* 2^16 */

std::vector<uint16_t> m_lines;

typedef enum 
{
    POS_UNKNOWN = 0,
    POS_WIN = 1,
    POS_LOST = 2
} pos_t;

/**     
 * Converts a given line segment (start, direction, length)
 * into a 16-bit mask.
 */     
static uint16_t makeLine(int x, int y, int dx, int dy, int size)
{
    uint16_t res = 0;
    for (int i=0; i<size; ++i)
    {
        res |= 1UL << (x*BOARD_SIZE+y);
        x += dx;
        y += dy;
    }   
    return res;
} // end of makeLine


static bool isBoardDead(uint16_t pos)
{
    for (uint16_t lineMask : m_lines)
    {
        if ((pos & lineMask) == lineMask)
        {
            return true;
        }
    }
    return false;

}; // end of isBoardDead

static void printBoard(uint16_t pos)
{
    for (unsigned int i=0; i<NUM_SQUARES; ++i)
    {
        if (i%BOARD_SIZE == 0 && i>0)
            std::cout << std::endl;
        uint16_t mask = 1UL << i;
        if (pos & mask)
            std::cout << "x";
        else
            std::cout << ".";
    }
}; // end of printBoard

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

    /*
    std::cout << m_lines.size() << std::endl;
    for (unsigned int i=0; i<m_lines.size(); ++i)
    {
        printBoard(m_lines[i]);
        std::cout << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */

}; // end of init

int main()
{
    init();

    pos_t egtb[NUM_POSITIONS];
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
                    uint16_t mask = 1UL << i;
                    if (pos & mask)
                        continue;
                    // Now we have a legal move

                    uint16_t new_pos = pos | mask;
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

    int numWins = 0;
    int numLost = 0;
    int numDead = 0;
    for (unsigned int pos=0; pos<NUM_POSITIONS; ++pos)
    {
        /*
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
        */

        if (!isBoardDead(pos))
        {
            if (egtb[pos] == POS_WIN)
                numWins++;
            else if (egtb[pos] == POS_LOST)
                numLost++;
        }
        else
            numDead++;
    }

    std::cout << "Wins = " << numWins << std::endl;
    std::cout << "Lost = " << numLost << std::endl;
    std::cout << "Dead = " << numDead << std::endl;

    return 0;
}
