#ifndef NTTTPLAYERMIKE_H
#define NTTTPLAYERMIKE_H

#include "NTTTPlayer.h"
#include <SDL.h>

constexpr bool DEBUG_MIKE = false;

/**
 * The search state of MikeBot: every board as a bitmask of its unmarked
 * squares, and the alpha-beta search over them.
 */
class MikeSearch {
public:
    void setVersion(int version) { m_version = version; }

    int getVersion() const { return m_version; }

    /**
     * Run in the initialization phase of the game.
     */
    void init(const NTTTGame& game);

    /**
     * Return the best move.
     */
    NTTTMove findMove(const NTTTGame& game);

    /**
     * Whether the tablebase says that the player to move wins the empty board.
     * Only known with a tablebase, one board and version 3 or later.
     */
    bool isEmptyBoardWon(bool& won) const;

private: // These methods are used internally only.
    /**
     * Whether a board is dead. Like the entries of m_unmarked, unmarked has a
     * bit set for each unmarked square, so a line is complete when none of its
     * bits are set.
     */
    bool isBoardDead(uint64_t unmarked) const;

    /**
     * For each line through the unmarked square sq, adds one to cnt[] at an
     * index given by how many marks the line still needs. unmarked has a bit
     * set for each unmarked square.
     */
    void getLineCounts(uint64_t unmarked, uint64_t sq, int cnt[4]) const;

    /**
     * Converts to internal representation
     * Returns number of boards still alive.
     */
    int makeBits(const NTTTGame& game);

    /**
     * Make a move.
     */
    void makeMove(int boardNumber, uint64_t mask);

    /**
     * Undo a move.
     */
    void undoMove(int boardNumber, uint64_t mask);

    /**
     * Return an estimate of the current position
     */
    int evaluate() const;

    /**
     * Perform an alpha beta search depth moves deep.
     * Returns the score of the position for the player to move.
     */
    int alphaBeta(int alpha, int beta, int depth);

    /**
     * Return an estimate of a single alive board, for version 6. It classifies
     * the unmarked squares by the lines through them, and looks up the parity
     * of the number of squares in each class in VALUES_V6.
     */
    int evaluateBoardV6(uint64_t board) const;

    /**
     * Clears the cache of evaluateBoardV6() results.
     */
    void clearCache();

    /**
     * Loads the endgame tablebase for the current board size and line size.
     * It is kept across games, and only reloaded when the sizes change.
     * Leaves m_egtb empty if there is no usable tablebase.
     */
    void loadEgtb();

private:
    static constexpr int CACHE_SIZE = 65536 * 16;
    // Cache of evaluateBoardV6() results, and statistics about it.
    mutable std::vector<uint64_t> m_cacheBoard;
    mutable std::vector<int> m_cacheValue;

    mutable int m_statLookup    = 0;
    mutable int m_statFound     = 0;
    mutable int m_statWrote     = 0;
    mutable int m_statCollision = 0;

    int m_boardCount = 0;
    int m_boardSize  = 0;
    int m_lineSize   = 0;
    int m_numSquares = 0;
    int m_nodes      = 0;
    std::vector<uint64_t> m_unmarked;
    std::vector<uint64_t> m_lines;
    bool m_debug  = DEBUG_MIKE;
    int m_version = 6;

    std::vector<uint8_t> m_egtb;                 // One bit per position. Empty if there is no tablebase.
    int m_egtbBoardSize = 0, m_egtbLineSize = 0; // The sizes m_egtb was loaded for.
}; // end of class MikeSearch


class NTTTPlayerMike : public NTTTPlayer {
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
    virtual NTTTMove chooseMove(const NTTTGame& game);

    virtual std::string getName();

    void setVersion(int version) { m_search.setVersion(version); }

    virtual ~NTTTPlayerMike() {}

private:
    MikeSearch m_search;
    bool m_debug = DEBUG_MIKE;
}; // end of class NTTTPlayerMike

#endif // NTTTPLAYERMIKE_H
