#ifndef NTTTBOARD_H
#define NTTTBOARD_H

#include <SDL.h>
#include "GUI/Texture.h"
#include <vector>
#include <iostream>

struct Line {
    int x, y, dx, dy;
};


class NTTTBoard {
public:
    /**
     * An enum capable of describing whether or not a board is alive.
     */
    enum State {
        DEAD,
        ALIVE
    };

    /**
     * An enum capable of describing the state of a square.
     */
    enum SquareState {
        BLUE,
        RED,
        UNMARKED
    };

    /**
     * @return A State enum representing whether or not a board is alive.
     */
    State getCurrentState() const {
        return m_state;
    }

    /**
     * @return A two-dimensional array representing the states of the squares.
     */
    const std::vector<std::vector<SquareState>>& getSquareStates() const {
        return m_squareStates;
    }

    bool isLineComplete(int x, int y, int dx, int dy) const {
        for (int i = 0; i < m_lineSize; ++i) {
            if (m_squareStates[x][y] == UNMARKED)
                return false;
            x += dx;
            y += dy;
        }
        return true;
    } // end of isLineComplete

    /**
     * Marks a square, or clears it again when state is UNMARKED, and then
     * finds out whether the board is alive.
     * @return Whether the board is still alive, i.e. has no complete line.
     */
    bool makeMove(int squareX, int squareY, SquareState state);

    /**
     * Clears every square, making the board alive.
     */
    void reset(int boardSize, int lineSize);

    /**
     * Prints the board one row per line: '.' for an unmarked square, 'x' for
     * blue and 'X' for red. A dead board is printed as all '$'.
     */
    friend std::ostream& operator<<(std::ostream& os, const NTTTBoard& rhs) {
        int boardSize = rhs.m_squareStates.size();
        for (int y = 0; y < boardSize; ++y) {
            for (int x = 0; x < boardSize; ++x) {
                if (rhs.m_state == DEAD)
                    os << "$";
                else
                    switch (rhs.m_squareStates[x][y]) {
                        case UNMARKED:
                            os << ".";
                            break;
                        case BLUE:
                            os << "x";
                            break;
                        case RED:
                            os << "X";
                            break;
                    }
            }
            os << std::endl;
        }
        return os;
    }

    void render(SDL_Renderer *renderer, Texture *redCross, Texture *blueCross, const int x, const int y, const unsigned int size) const;

private:
    int m_lineSize = 0;
    State m_state  = DEAD;
    std::vector<std::vector<SquareState>> m_squareStates;
    Line m_deadLine = {};
    void renderAntiDiagonalLine(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const;
    void renderDiagonalLine(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const;
}; // end of class NTTTBoard

#endif // NTTTBOARD_H
