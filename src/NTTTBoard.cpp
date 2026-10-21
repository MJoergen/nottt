#include <assert.h>
#include <SDL.h>
#include "GUI/Texture.h"

#include "NTTTGame.h"
#include "NTTTBoard.h"
#include "NTTTManager.h"

bool NTTTBoard::makeMove(int squareX, int squareY, SquareState state) {

    if (state == UNMARKED) {
        assert(m_squareStates[squareX][squareY] != UNMARKED);
    } else {
        assert(m_state == ALIVE);
        assert(m_squareStates[squareX][squareY] == UNMARKED);
    }


    m_squareStates[squareX][squareY] = state;

    int boardSize = m_squareStates.size();
    for (int x = 0; x < boardSize; ++x) {
        for (int y = 0; y < boardSize; ++y) {
            if (x + m_lineSize <= boardSize) // Horizontal
                if (isLineComplete(x, y, 1, 0)) {
                    m_deadLine = {x, y, 1, 0};
                    m_state    = DEAD;
                    return false;
                }
            if (y + m_lineSize <= boardSize) // Vertical
                if (isLineComplete(x, y, 0, 1)) {
                    m_deadLine = {x, y, 0, 1};
                    m_state    = DEAD;
                    return false;
                }
            if (x + m_lineSize <= boardSize && y + m_lineSize <= boardSize) // Diagonal
                if (isLineComplete(x, y, 1, 1)) {
                    m_deadLine = {x, y, 1, 1};
                    m_state    = DEAD;
                    return false;
                }
            if (x >= m_lineSize - 1 && y + m_lineSize <= boardSize) // Diagonal
                if (isLineComplete(x, y, -1, 1)) {
                    m_deadLine = {x, y, -1, 1};
                    m_state    = DEAD;
                    return false;
                }
        }
    }
    m_state = ALIVE;
    return true;
} // end of makeMove

void NTTTBoard::reset(int boardSize, int lineSize) {
    m_squareStates.resize(boardSize);
    for (std::vector<SquareState>& row : m_squareStates) {
        row.resize(boardSize);
        for (SquareState& square : row) {
            square = UNMARKED;
        }
    }
    m_state    = ALIVE;
    m_lineSize = lineSize;
    m_deadLine = {};
}

void NTTTBoard::renderAntiDiagonalLine(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const {
    for (int index = 0; index < thickness / 2; index++) {
        SDL_RenderDrawLine(renderer, x, y + index, x - width + index, y + width);
        SDL_RenderDrawLine(renderer, x - index, y, x - width, y + width - index);
    }
    SDL_RenderDrawLine(renderer, x, y, x - width, y + width);
}

void NTTTBoard::renderDiagonalLine(SDL_Renderer *renderer, const int thickness, const int x, const int y, const int width) const {
    for (int index = 0; index < thickness / 2; index++) {
        SDL_RenderDrawLine(renderer, x, y + index, x + width - index, y + width);
        SDL_RenderDrawLine(renderer, x + index, y, x + width, y + width - index);
    }
    SDL_RenderDrawLine(renderer, x, y, x + width, y + width);
}

void NTTTBoard::render(SDL_Renderer *renderer, Texture *redCross, Texture *blueCross, const int x, const int y, const unsigned int size) const {
    int boardSize  = m_squareStates.size();
    int squareSize = size / boardSize;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int index = 0; index < boardSize - 1; index++) {
        SDL_RenderDrawLine(renderer,
                           x + squareSize * (index + 1), y,
                           x + squareSize * (index + 1), y + size);
        SDL_RenderDrawLine(renderer,
                           x + size, y + squareSize * (index + 1),
                           x, y + squareSize * (index + 1));
    }
    for (int squareX = 0; squareX < boardSize; squareX++) {
        for (int squareY = 0; squareY < boardSize; squareY++) {
            if (m_squareStates[squareX][squareY] == RED)
                redCross->render(renderer,
                                 x + squareX * squareSize, y + squareY * squareSize, squareSize, squareSize);
            else if (m_squareStates[squareX][squareY] == BLUE)
                blueCross->render(renderer,
                                  x + squareX * squareSize, y + squareY * squareSize, squareSize, squareSize);
        }
    }
    if (m_state == DEAD) {
        if (m_deadLine.dx == m_deadLine.dy) {
            renderDiagonalLine(renderer,
                               squareSize / 4, x + m_deadLine.x * squareSize,
                               y + m_deadLine.y * squareSize, m_lineSize * squareSize);
        } else if (m_deadLine.dx == -1) {
            renderAntiDiagonalLine(renderer,
                                   squareSize / 4, x + (m_deadLine.x + 1) * squareSize,
                                   y + m_deadLine.y * squareSize, m_lineSize * squareSize);
        } else if (m_deadLine.dx == 0) {
            SDL_Rect rect = {
                (int)(x + (m_deadLine.x + 0.375) * squareSize), y + m_deadLine.y * squareSize,
                squareSize / 4, m_lineSize * squareSize};
            SDL_RenderFillRect(renderer, &rect);
        } else if (m_deadLine.dx == 1) {
            SDL_Rect rect = {
                x + m_deadLine.x * squareSize, (int)(y + (m_deadLine.y + 0.375) * squareSize),
                m_lineSize * squareSize, squareSize / 4};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
