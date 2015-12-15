#ifndef _NTTTBOARD_H_
#define _NTTTBOARD_H_

#include <vector>
#include <iostream>

class NTTTBoard {
    public:

        /**
         * An enum capable of describing whether or not a board is alive.
         * @author Ice
         */
        enum State {
            DEAD, ALIVE
        };

        /**
         * An enum capable of describing the state of a square.
         * @author Ice
         */
        enum SquareState {
            BLUE, RED, UNMARKED
        };

        /**
         * @return A State enum representing whether or not a board is alive.
         */
        State getCurrentState() const
        {
            return m_state;
        }

        /**
         * @return A two-dimensional array representing the states of the squares.
         */
        std::vector< std::vector<SquareState> > getSquareStates() const
        {
            return m_squareStates;
        }

        /**
         */
        void makeMove(int squareX, int squareY, SquareState state)
        {
            m_squareStates[squareX][squareY] = state;
        }

        /**
         */
        void reset(int boardSize)
        {
            m_squareStates.resize(boardSize);
            for (std::vector<SquareState>& row : m_squareStates)
            {
                row.resize(boardSize);
                for (SquareState& square : row)
                {
                    square = UNMARKED;
                }
            }
        }

        /**
         */
        void Dump() const
        {
            for (const std::vector<SquareState>& row : m_squareStates)
            {
                for (const SquareState& square : row)
                {
                    switch (square)
                    {
                        case UNMARKED : std::cout << "."; break;
                        case BLUE     : std::cout << "x"; break;
                        case RED      : std::cout << "X"; break;
                    }
                }
                std::cout << std::endl;
            }
        }

    private:
        State m_state;
        std::vector< std::vector<SquareState> > m_squareStates;
}; // end of class NTTTBoard

#endif // _NTTTBOARD_H_

