#ifndef NTTTMOVE_H
#define NTTTMOVE_H

class NTTTMove {
private:
    int m_boardNumber;
    int m_squareX;
    int m_squareY;

public:
    /**
     * @param boardNumber An integer representing which of the boards the move is performed on.
     * @param squareX An integer representing the X-component of the selected square.
     * @param squareY An integer representing the Y-component of the selected square.
     */
    NTTTMove(int boardNumber, int squareX, int squareY) {
        m_boardNumber = boardNumber;
        m_squareX     = squareX;
        m_squareY     = squareY;
    }

    /**
     * @return An integer representing which of the boards the move is performed on.
     */
    int getBoardNumber() const {
        return m_boardNumber;
    }

    /**
     * @return An integer representing the X-component of the selected square.
     */
    int getSquareX() const {
        return m_squareX;
    }

    /**
     * @return An integer representing the Y-component of the selected square.
     */
    int getSquareY() const {
        return m_squareY;
    }

    /**
     * Make an ASCII dump
     */
    friend std::ostream& operator<<(std::ostream& os, const NTTTMove& rhs) {
        return os << "(" << rhs.m_boardNumber << ":" << rhs.m_squareX << "," << rhs.m_squareY << ")";
    }

}; // end of class NTTTMove

#endif // NTTTMOVE_H
