#ifndef _NTTTPLAYERMIKE_H_
#define _NTTTPLAYERMIKE_H_

#include "NTTTPlayer.h"
#include <SDL.h>

/**
 * A representation of the board using bitmasks
 */
class Board
{
    public:
        /**
         * Run in the initialization phase of the game.
         */
        void init(const NTTTGame& game);

        /**
         */
        bool isBoardDead(uint64_t bits) const;

        /**
         * Converts to internal representation
         */
        void makeBits(const NTTTGame& game);

        /**
         * Make a move.
         */
        void makeMove(int board, int bit);

        /**
         * Undo a move.
         */
        void undoMove(int board, int bit);

        /**
         * Return an estimate of the current position
         */
        int evaluate() const;
    
        /**
         * Perform an alpha beta search
         * Returns a value
         */
        int alphaBeta(int alpha, int beta, int level);

        /**
         * Return the best move.
         */
        NTTTMove findMove();

    private:
        int m_boardCount;
        int m_boardSize;
        int m_lineSize;
        int m_maxBits;
        int m_nodes;
        std::vector<uint64_t> m_bits;
        std::vector<uint64_t> m_lines;
}; // end of class Board


class NTTTPlayerMike : public NTTTPlayer {
    public:

        /**
         * Run in the initialization phase of the game.
         * @param game An instance of the NTTTGame.
         * @return The player's/bot's choice of order.
         */
        virtual OrderChoice chooseOrder(const NTTTGame& game);

        /**
         * Run continually as the game progresses.
         * @param game An instance of the NTTTGame.
         * @return The player's/bot's move.
         */
        virtual NTTTMove performMove(const NTTTGame& game);

        /**
         * The destructor must always be made virtual
         */
        virtual ~NTTTPlayerMike() {}

    private:
        Board m_board;
}; // end of class NTTTPlayerMike

#endif // _NTTTPLAYERMIKE_H_

