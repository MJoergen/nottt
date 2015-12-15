#ifndef _NTTTBOARD_H_
#define _NTTTBOARD_H_

#include <vector>

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

    private:
        State m_state;
        std::vector< std::vector<SquareState> > m_squareStates;
}; // end of class NTTTBoard

#endif // _NTTTBOARD_H_

