#ifndef NTTTPLAYERICE_H
#define NTTTPLAYERICE_H

#include "NTTTPlayer.h"
#include <cstdint>
#include <vector>

#define NEW_VERSION 1

/**
 * This is an abstract base class that is to be derived from.
 * To implement a player, you should write something like:
 *
 * #include "NTTTPlayer.h"
 * class HTTPPlayerIce : HTTPPlayer
 * {
 * }
 *
 */

class NTTTPlayerIce : public NTTTPlayer {
private:
    int m_boardCount = 0, m_boardSize = 0, m_lineSize = 0;
    std::vector<uint8_t> m_egtb;                 // One bit per position. Empty if there is no tablebase.
    int m_egtbBoardSize = 0, m_egtbLineSize = 0; // The sizes m_egtb was loaded for.
    void loadEgtb();
    std::vector<NTTTMove> getPossibleMoves(const NTTTGame& game) const;
    const bool isNumberEven(const int number) const;

#if NEW_VERSION
    const bool isBoardWon(const NTTTBoard board) const;
    const NTTTMove playBestMove(const int boardNumber, const NTTTBoard& board, const unsigned int otherBoards) const;
#else
    const bool tryMove(const NTTTGame game, const int boardIndex, const int squareX, const int squareY) const;
#endif

public:
    /**
     * Run in the initialization phase of the game.
     * @param game An instance of the NTTTGame.
     * @return The player's/bot's choice of order.
     */
    virtual OrderChoice chooseOrder(const NTTTGame& game);

    /**
     * Run continually as the game progresses.
     * @param game An instance of the NTTTGame.
     * @return The player's/bot's move.
     */
    virtual NTTTMove performMove(const NTTTGame& game);

    virtual std::string getName();

    /**
     * The destructor must always be made virtual
     */
    virtual ~NTTTPlayerIce() {}
}; // end of class NTTTPlayerIce

#endif // NTTTPLAYERICE_H
