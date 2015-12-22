#ifndef _NTTTPLAYER_H_
#define _NTTTPLAYER_H_

#include "NTTTGame.h"
#include "NTTTMove.h"

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

class NTTTPlayer {
    public:

        /**
         * An enum capable of describing the player's/bot's choice of order.
         * @author Ice
         */
        enum OrderChoice {
            FIRST, LAST, UNDECIDED
        };

        /**
         * Run in the initialization phase of the game.
         * @param game An instance of the NTTTGame.
         * @return The player's/bot's choice of order.
         */
        virtual OrderChoice chooseOrder(const NTTTGame& game) = 0;

        /**
         * Run continually as the game progresses.
         * @param game An instance of the NTTTGame.
         * @return The player's/bot's move.
         */
        virtual NTTTMove performMove(const NTTTGame& game) = 0;

		virtual std::string getName() = 0;

        /**
         * The destructor must always be made virtual
         */
        virtual ~NTTTPlayer() {}
}; // end of class NTTTPlayer 

#endif // _NTTTPLAYER_H_

