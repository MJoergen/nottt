#ifndef _NTTTGAME_H_
#define _NTTTGAME_H_

#include "NTTTBoard.h"

class NTTTGame {
    public:

        /**
         * @return An integer representing the number of boards in the game.
         */
        int getBoardCount();

        /** 
         * @return An integer representing the size. (e.g. 4, which represents a board size of 4x4)
         */
        int getBoardSize();

        /**
         * @return An integer representing the number of consecutive marks before a board has died.
         */
        int getLineSize();

        /**
         * @return An array of Board objects representing the current setting of the game.
         */
        std::vector<NTTTBoard> getBoards();

}; // end of class NTTTGame 

#endif // _NTTTGAME_H_

