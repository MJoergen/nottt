#ifndef _NTTTBOARD_H_
#define _NTTTBOARD_H_

#include <vector>
#include <iostream>
#include <SDL.h>
#include "GUI/Texture.h"

extern SDL_Renderer *g_renderer;
extern Texture *g_redCross, *g_blueCross;

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
        void makeMove(int squareX, int squareY, SquareState state)
        {
            m_squareStates[squareX][squareY] = state;

			int boardSize = m_squareStates.size();
            for (int x=0; x<boardSize; ++x)
            {
                for (int y=0; y<boardSize; ++y)
                {
                    if (x+m_lineSize <= boardSize) // Horizontal
                        if (checkLine(x, y, 1, 0))
                        {
                            m_state = DEAD;
                            return;
                        }
                    if (y+m_lineSize <= boardSize) // Vertical
                        if (checkLine(x, y, 0, 1))
                        {
                            m_state = DEAD;
                            return;
                        }
                    if (x+m_lineSize <= boardSize && y+m_lineSize <= boardSize) // Diagonal
                        if (checkLine(x, y, 1, 1))
                        {
                            m_state = DEAD;
                            return;
                        }
                    if (x >= m_lineSize-1 && y+m_lineSize <= boardSize) // Diagonal
                        if (checkLine(x, y, -1, 1))
                        {
                            m_state = DEAD;
                            return;
                        }
                }
            }
        } // end of makeMove

        /**
         */
        void reset(int boardSize, int lineSize)
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
            m_state = ALIVE;
            m_lineSize = lineSize;
        }

        /**
         */
        friend std::ostream& operator <<(std::ostream &os, const NTTTBoard &rhs)
        {
            for (const std::vector<SquareState>& row : rhs.m_squareStates)
            {
                for (const SquareState& square : row)
                {
                    switch (square)
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

		void renderBoard(const unsigned int x, const unsigned int y, const unsigned int size) const {
			unsigned int boardSize = m_squareStates.size();
			unsigned int squareSize = size / boardSize;
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
			for (unsigned int index = 0; index < boardSize - 1; index++){
				SDL_RenderDrawLine(g_renderer, x + squareSize * (index + 1), y, x + squareSize * (index + 1), y + size);
				SDL_RenderDrawLine(g_renderer, x + size, y + squareSize * (index + 1), x, y + squareSize * (index + 1));
			}
			for (unsigned int x_i = 0; x_i < boardSize; x_i++){
				for (unsigned int y_i = 0; y_i < boardSize; y_i++){
					if (m_squareStates[x_i][y_i] == RED)
						g_redCross->renderTexture(x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
					else if (m_squareStates[x_i][y_i] == BLUE)
						g_blueCross->renderTexture(x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
				}
			}
		}

    private:
        int m_lineSize;
        State m_state;
        std::vector< std::vector<SquareState> > m_squareStates;
}; // end of class NTTTBoard

#endif // _NTTTBOARD_H_

