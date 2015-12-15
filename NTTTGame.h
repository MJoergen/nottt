#ifndef _NTTTGAME_H_
#define _NTTTGAME_H_

#include "NTTTBoard.h"

class NTTTGame {
    public:

        /**
         * @return An integer representing the number of boards in the game.
         */
        int getBoardCount() const {return m_boards.size();}

        /** 
         * @return An integer representing the size. (e.g. 4, which represents a board size of 4x4)
         */
        int getBoardSize() const {return m_boards[0].getSquareStates().size();}

        /**
         * @return An integer representing the number of consecutive marks before a board has died.
         */
        int getLineSize() const {return m_lineSize;}

        /**
         * @return An array of Board objects representing the current setting of the game.
         */
        std::vector<NTTTBoard> getBoards() const {return m_boards;}

        /**
         * Start a new game
         */
        void NewGame(int boardCount, int boardSize, int lineSize)
        {
            m_boards.resize(boardCount);
            for (auto it=m_boards.begin(); it != m_boards.end(); ++it)
            {
                it->reset(boardSize);
            }
            m_lineSize = lineSize;
        }

    private:
        int m_lineSize;
        std::vector<NTTTBoard> m_boards;

}; // end of class NTTTGame 

#endif // _NTTTGAME_H_

