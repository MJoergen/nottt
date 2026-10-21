#ifndef NTTTPLAYERICE_H
#define NTTTPLAYERICE_H

#include "NTTTPlayer.h"
#include <cstdint>
#include <vector>

/**
 * IceBot. It plays from the endgame tablebase when there is one for the
 * board size and line size.
 */

class NTTTPlayerIce : public NTTTPlayer {
private:
    int m_boardCount = 0, m_boardSize = 0, m_lineSize = 0;
    std::vector<uint8_t> m_egtb;                 // One bit per position. Empty if there is no tablebase.
    int m_egtbBoardSize = 0, m_egtbLineSize = 0; // The sizes m_egtb was loaded for.
    void loadEgtb();
    const bool isNumberEven(const int number) const;
    const bool isBoardWon(const NTTTBoard board) const;
    const NTTTMove playBestMove(const int boardNumber, const NTTTBoard& board, const unsigned int otherBoards) const;

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

    virtual ~NTTTPlayerIce() {}
}; // end of class NTTTPlayerIce

#endif // NTTTPLAYERICE_H
