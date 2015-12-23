#include <assert.h>
#include <SDL.h>
#include "GUI/Texture.h"

#include "NTTTGame.h"
#include "NTTTBoard.h"
#include "NTTTManager.h"

/**
*/
bool NTTTBoard::makeMove(int squareX, int squareY, SquareState state)
{
    
    if (state == UNMARKED)
    {
        assert(m_squareStates[squareX][squareY] != UNMARKED);
    }
    else
    {
        assert(m_state == ALIVE);
        assert(m_squareStates[squareX][squareY] == UNMARKED);
    }
    

    m_squareStates[squareX][squareY] = state;

    int boardSize = m_squareStates.size();
    for (int x=0; x<boardSize; ++x)
    {
        for (int y=0; y<boardSize; ++y)
        {
            if (x+m_lineSize <= boardSize) // Horizontal
                if (checkLine(x, y, 1, 0))
				{
					m_line = { x, y, 1, 0 };
					m_state = DEAD;
					return false;
                }
            if (y+m_lineSize <= boardSize) // Vertical
                if (checkLine(x, y, 0, 1))
				{
					m_line = { x, y, 0, 1 };
					m_state = DEAD;
					return false;
                }
            if (x+m_lineSize <= boardSize && y+m_lineSize <= boardSize) // Diagonal
                if (checkLine(x, y, 1, 1))
				{
					m_line = { x, y, 1, 1 };
					m_state = DEAD;
                    return false;
                }
            if (x >= m_lineSize-1 && y+m_lineSize <= boardSize) // Diagonal
                if (checkLine(x, y, -1, 1))
				{
					m_line = { x, y, -1, 1 };
					m_state = DEAD;
					return false;
                }
        }
    }
	m_state = ALIVE;
	return true;
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
	m_line = {};
}

void NTTTBoard::renderDiagonalLineRight(const int thickness, const int x, const int y, const int width) const{
	for (int index = 0; index < thickness / 2; index++){
		SDL_RenderDrawLine(g_NtttManager.g_renderer, x, y + index, x - width + index, y + width);
		SDL_RenderDrawLine(g_NtttManager.g_renderer, x - index, y, x - width, y + width - index);
	}
	SDL_RenderDrawLine(g_NtttManager.g_renderer, x, y, x - width, y + width);
}

void NTTTBoard::renderDiagonalLineLeft(const int thickness, const int x, const int y, const int width) const{
	for (int index = 0; index < thickness / 2; index++){
		SDL_RenderDrawLine(g_NtttManager.g_renderer, x, y + index, x + width - index, y + width);
		SDL_RenderDrawLine(g_NtttManager.g_renderer, x + index, y, x + width, y + width - index);
	}
	SDL_RenderDrawLine(g_NtttManager.g_renderer, x, y, x + width, y + width);
}

void NTTTBoard::renderBoard(const /* unsigned */ int x, const /* unsigned */ int y, const unsigned int size) const 
{
    /* unsigned */ int boardSize = m_squareStates.size();
    /* unsigned */ int squareSize = size / boardSize;
    SDL_SetRenderDrawColor(g_NtttManager.g_renderer, 0, 0, 0, 255);
    for (/* unsigned */ int index = 0; index < boardSize - 1; index++){
        SDL_RenderDrawLine(g_NtttManager.g_renderer,
                x + squareSize * (index + 1), y,
                x + squareSize * (index + 1), y + size);
        SDL_RenderDrawLine(g_NtttManager.g_renderer,
                x + size, y + squareSize * (index + 1),
                x, y + squareSize * (index + 1));
    }
    for (/* unsigned */ int x_i = 0; x_i < boardSize; x_i++){
        for (/* unsigned */ int y_i = 0; y_i < boardSize; y_i++){
            if (m_squareStates[x_i][y_i] == RED)
                g_NtttManager.g_redCross->renderTexture(
                        x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
            else if (m_squareStates[x_i][y_i] == BLUE)
                g_NtttManager.g_blueCross->renderTexture(
                        x + x_i * squareSize, y + y_i * squareSize, squareSize, squareSize);
        }
    }
	if (m_state == DEAD){
		if (m_line.dx == m_line.dy){
			renderDiagonalLineLeft(
                    squareSize / 4, x + m_line.x * squareSize,
                    y + m_line.y * squareSize, m_lineSize * squareSize);
		}
		else if (m_line.dx == -1){
			renderDiagonalLineRight(
                    squareSize / 4, x + (m_line.x + 1) * squareSize,
                    y + m_line.y * squareSize, m_lineSize * squareSize);
		}
		else if (m_line.dx == 0){
			SDL_Rect rect = { 
                (int) (x + (m_line.x + 0.375) * squareSize), y + m_line.y * squareSize,
                squareSize / 4, m_lineSize * squareSize };
			SDL_RenderFillRect(g_NtttManager.g_renderer, &rect);
		}
		else if (m_line.dx == 1){
			SDL_Rect rect = {
                x + m_line.x * squareSize, (int)(y + (m_line.y + 0.375) * squareSize),
                m_lineSize * squareSize, squareSize / 4 };
			SDL_RenderFillRect(g_NtttManager.g_renderer, &rect);
		}
	}
}

