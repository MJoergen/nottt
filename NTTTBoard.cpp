#include <SDL.h>
#include "GUI/Texture.h"

#include "NTTTGame.h"
#include "NTTTBoard.h"
#include "NTTTManager.h"

/**
*/
void NTTTBoard::makeMove(int squareX, int squareY, SquareState state)
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
void NTTTBoard::reset(int boardSize, int lineSize)
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


void NTTTBoard::renderBoard(const unsigned int x, const unsigned int y, const unsigned int size) const 
{
    unsigned int boardSize = m_squareStates.size();
    unsigned int squareSize = size / boardSize;
    SDL_SetRenderDrawColor(g_NtttManager.g_renderer, 0, 0, 0, 255);
    for (unsigned int index = 0; index < boardSize - 1; index++){
        SDL_RenderDrawLine(g_NtttManager.g_renderer, x + squareSize * (index + 1), y, x + squareSize * (index + 1), y + size);
        SDL_RenderDrawLine(g_NtttManager.g_renderer, x + size, y + squareSize * (index + 1), x, y + squareSize * (index + 1));
    }
    for (unsigned int x_i = 0; x_i < boardSize; x_i++){
        for (unsigned int y_i = 0; y_i < boardSize; y_i++){
            if (m_squareStates[x_i][y_i] == RED)
                g_NtttManager.g_redCross->renderTexture(x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
            else if (m_squareStates[x_i][y_i] == BLUE)
                g_NtttManager.g_blueCross->renderTexture(x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
        }
    }
}

