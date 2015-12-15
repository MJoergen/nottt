#include "NTTTPlayerMike.h"

/**
 * Run in the initialization phase of the game.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's choice of order.
 */
NTTTPlayer::OrderChoice NTTTPlayerMike::chooseOrder(const NTTTGame& game)
{
    return UNDECIDED;
} // end of chooseOrder


/**
 * Run continually as the game progresses.
 * @param game An instance of the NTTTGame.
 * @return The player's/bot's move.
 */
NTTTMove NTTTPlayerMike::performMove(const NTTTGame& game)
{
    int boardCount = game.getBoardCount();
    int boardSize  = game.getBoardSize();

    while (true)
    {
        int boardNum = rand()%boardCount;
        const std::vector<NTTTBoard>& boards = game.getBoards();
        const NTTTBoard& board = boards[boardNum];
        NTTTBoard::State state = board.getCurrentState();
        if (state != NTTTBoard::ALIVE)
            continue;

        const std::vector< std::vector<NTTTBoard::SquareState> >& squareStates = board.getSquareStates();

        while (true)
        {
            int squareX = rand()%boardSize;
            int squareY = rand()%boardSize;

            if (squareStates[squareX][squareY] != NTTTBoard::UNMARKED)
                continue;

            return NTTTMove(boardNum, squareX, squareY);
        }
    }

    return NTTTMove(0, 0, 0); // Never used
} // end of performMove

