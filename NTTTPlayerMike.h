#ifndef _NTTTPLAYERMIKE_H_
#define _NTTTPLAYERMIKE_H_

#include "NTTTPlayer.h"
#include <SDL.h>

#define DEBUG_MIKE false

#if 0
/*
 * It is important that the size of the enum is only 1 byte.
 */
typedef enum : char
{
    POS_UNKNOWN = 0,
    POS_WIN = 1,
    POS_LOST = 2
} val_t;
#endif

/**
 * A representation of the board using bitmasks
 */
class Board
{
    public:
        Board() : 
            m_filePath(""),
            m_fileSize(0),
            m_egtb(nullptr) {}
        ~Board() {
            if (m_egtb)
            {
                delete [] m_egtb;
                m_egtb = nullptr;
            }
        }

        void setVersion(int version) {m_version = version;}

        int getVersion() const {return m_version;}

        /**
         * Run in the initialization phase of the game.
         */
        void init(const NTTTGame& game);

        /**
         * Return the best move.
         */
        NTTTMove findMove(const NTTTGame& game);


    private: // These methods are used internally only.

        /**
         */
        bool isBoardDead(uint64_t bits) const;

        /**
         */
        void getLineCounts(uint64_t pos, uint64_t sq, int cnt[4]) const;

        /**
         * Converts to internal representation
         * Returns number of boards still alive.
         */
        int makeBits(const NTTTGame& game);

        /**
         * Make a move.
         */
        void makeMove(int board, uint64_t mask);

        /**
         * Undo a move.
         */
        void undoMove(int board, uint64_t mask);

        /**
         * Return an estimate of the current position
         */
        int evaluate() const;
    
        /**
         * Perform an alpha beta search
         * Returns a value
         */
        int alphaBeta(int alpha, int beta, int level);

    private:
        int m_boardCount;
        int m_boardSize;
        int m_lineSize;
        int m_maxBits;
        int m_nodes;
        std::vector<uint64_t> m_bits;
        std::vector<uint64_t> m_lines;
        bool m_debug = DEBUG_MIKE;
        int  m_version = 4;

        std::string m_filePath;
        int         m_fileSize;
        uint8_t     *m_egtb;
}; // end of class Board


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
        virtual NTTTMove performMove(const NTTTGame& game);

		virtual std::string getName();

        void setVersion(int version) {m_board.setVersion(version);}

        /**
         * The destructor must always be made virtual
         */
        virtual ~NTTTPlayerMike() {}

    private:
        Board m_board;
        bool  m_debug = DEBUG_MIKE;
}; // end of class NTTTPlayerMike

#endif // _NTTTPLAYERMIKE_H_

