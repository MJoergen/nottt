#ifndef _NTTTGAME_H_
#define _NTTTGAME_H_

#include <string>
#include "NTTTBoard.h"
#include "NTTTMove.h"

class NTTTGame {
    public:

        /**
         * The largest supported board size. The bots store a board in 64 bits.
         */
        static const int MAX_BOARD_SIZE = 8;

        /**
         * Checks whether the game settings are supported.
         * @return An empty string if they are, otherwise a description of the problem.
         */
        static std::string checkSettings(int boardCount, int boardSize, int lineSize)
        {
            if (boardCount < 1)
                return "Board count must be at least 1";
            if (boardSize < 1 || boardSize > MAX_BOARD_SIZE)
                return "Board size must be between 1 and " + std::to_string(MAX_BOARD_SIZE);
            if (lineSize < 1 || lineSize > boardSize)
                return "Line size must be between 1 and the board size";
            return "";
        }

        /**
         * @return An integer representing the number of boards in the game.
         */
        int getBoardCount() const {return m_boards.size();}

        /** 
         * @return An integer representing the size. (e.g. 4, which represents a board size of 4x4)
         */
        int getBoardSize() const {return m_boards.empty() ? 0 : m_boards[0].getSquareStates().size();}

        /**
         * @return An integer representing the number of consecutive marks before a board has died.
         */
        int getLineSize() const {return m_lineSize;}

        /**
         * @return An array of Board objects representing the current setting of the game.
         */
        const std::vector<NTTTBoard>& getBoards() const {return m_boards;}

        /**
         * @return An array of Board objects representing the current setting of the game.
         */
        bool isActive() const
        {
            for (const NTTTBoard& board : m_boards)
            {
                if (board.getCurrentState() == NTTTBoard::ALIVE)
                    return true;
            }

            return false;
        }

        /**
         * Start a new game
         */
        void NewGame(int boardCount, int boardSize, int lineSize)
        {
            m_boards.resize(boardCount);
            for (auto it=m_boards.begin(); it != m_boards.end(); ++it)
            {
                it->reset(boardSize, lineSize);
            }
            m_lineSize = lineSize;
        }

        /**
         * @return Whether the move marks an unmarked square on a board that is alive.
         */
        bool isLegalMove(const NTTTMove& move) const
        {
            int boardNumber = move.getBoardNumber();
            int squareX = move.getSquareX();
            int squareY = move.getSquareY();
            int boardSize = getBoardSize();
            if (boardNumber < 0 || boardNumber >= getBoardCount() ||
                squareX < 0 || squareX >= boardSize || squareY < 0 || squareY >= boardSize)
                return false;

            const NTTTBoard& board = m_boards[boardNumber];
            return board.getCurrentState() == NTTTBoard::ALIVE &&
                board.getSquareStates()[squareX][squareY] == NTTTBoard::UNMARKED;
        }

        /**
         * Make a move. The move must be legal, see isLegalMove().
         */
        void makeMove(const NTTTMove& move, const NTTTBoard::SquareState& state)
        {
            int boardNumber = move.getBoardNumber();
            int squareX = move.getSquareX();
            int squareY = move.getSquareY();
            m_boards[boardNumber].makeMove(squareX, squareY, state);
        }

        /*
         * Undo a move
         */
        void undoMove(const NTTTMove& move)
        {
            int boardNumber = move.getBoardNumber();
            int squareX = move.getSquareX();
            int squareY = move.getSquareY();
            m_boards[boardNumber].makeMove(squareX, squareY, NTTTBoard::UNMARKED);
        }

        /**
         * ASCII dump the board configuration
         */
        friend std::ostream& operator <<(std::ostream &os, const NTTTGame &rhs)
        {
            for (const NTTTBoard& board : rhs.m_boards)
            {
                os << board << std::endl;
            }
            return os;
        }

    private:
        int m_lineSize = 0;
        std::vector<NTTTBoard> m_boards;

}; // end of class NTTTGame 

#endif // _NTTTGAME_H_

