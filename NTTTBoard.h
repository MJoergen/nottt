#ifndef _NTTTBOARD_H_
#define _NTTTBOARD_H_

#include <SDL.h>
#include "GUI/Texture.h"
#include <vector>
#include <iostream>

struct Line{
	int x, y, dx, dy;
};


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
        const std::vector< std::vector<SquareState> >& getSquareStates() const
        {
            return m_squareStates;
        }

        bool checkLine(int x, int y, int dx, int dy) const
        {
            for (int i=0; i<m_lineSize; ++i)
            {
                if (m_squareStates[x][y] == UNMARKED)
                    return false;
                x += dx;
                y += dy;
            }
            return true;
        } // end of checkLine

        /**
         */
        bool makeMove(int squareX, int squareY, SquareState state);

        /**
         */
        void reset(int boardSize, int lineSize);

        /**
         */
        friend std::ostream& operator <<(std::ostream &os, const NTTTBoard &rhs)
        {
            int boardSize = rhs.m_squareStates.size();
            for (int x=0; x<boardSize; ++x)
            {
                for (int y=0; y<boardSize; ++y)
                {
                    if (rhs.m_state == DEAD)
                        os << "$";
                    else
                        switch (rhs.m_squareStates[y][x])
                        {
                            case UNMARKED : os << "."; break;
                            case BLUE     : os << "x"; break;
                            case RED      : os << "X"; break;
                        }
                }
                os << std::endl;
            }
            return os;
        }

		void renderBoard(SDL_Renderer *renderer, Texture *redCross, Texture *blueCross, const /* unsigned */ int x, const /* unsigned */ int y, const unsigned int size) const;

    private:
        int m_lineSize;
        State m_state;
        std::vector< std::vector<SquareState> > m_squareStates;
		Line m_line;
		void renderDiagonalLineRight(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const;
		void renderDiagonalLineLeft(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const;
}; // end of class NTTTBoard

#endif // _NTTTBOARD_H_

