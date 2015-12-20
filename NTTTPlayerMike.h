#ifndef _NTTTPLAYERMIKE_H_
#define _NTTTPLAYERMIKE_H_

#include "NTTTPlayer.h"

/**
 * This is an abstract base class that is to be derived from.
 * To implement a player, you should write something like:
 *
 * #include "NTTTPlayer.h"
 * class HTTPPlayerMike : HTTPPlayer 
 * {
 * }
 *
 */

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
         * Initializes the player with the new board configuration.
         */
        virtual void NewGame(int boardCount, int boardSize, int lineSize);

        /**
         * The destructor must always be made virtual
         */
        virtual ~NTTTPlayerMike() {}

    private:
        std::vector<uint64_t> m_lines;
        std::vector<NTTTMove> m_moves;
        int m_boardCount;
        int m_boardSize;
        int m_lineSize;

        void genMoves(const NTTTGame& game);
}; // end of class NTTTPlayerMike

#endif // _NTTTPLAYERMIKE_H_

