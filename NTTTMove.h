#ifndef _NTTPMOVE_H_
#define _NTTPMOVE_H_

class NTTTMove {
    private:

        int m_boardNumber;
        int m_squareX;
        int m_squareY;

        /**
         * @param boardNumber An integer representing which of the boards the move is performed on.
         * @param squareX An integer representing the X-component of the selected square.
         * @param squareY An integer representing the Y-component of the selected square.
         */
    public:

        NTTTMove(int boardNumber, int squareX, int squareY) {
            m_boardNumber = boardNumber;
            m_squareX = squareX;
            m_squareY = squareY;
        }

        /**
         * @return An integer representing which of the boards the move is performed on.
         */
        int getBoardNumber() const {
            return m_boardNumber;
        }

        /**
         * Sets the integer representing the board
         * @param boardNumber An integer representing which of the boards the move is performed on.
         */
        void setBoardNumber(int boardNumber) {
            m_boardNumber = boardNumber;
        }

        /**
         * @return An integer representing the X-component of the selected square.
         */
        int getSquareX() const {
            return m_squareX;
        }

        /**
         * Sets the integer representing the X-compontent of the selected square
         * @param squareX An integer representing the X-component of the selected square.
         */
        void setSquareX(int squareX) {
            m_squareX = squareX;
        }

        /**
         * @return An integer representing the Y-component of the selected square.
         */
        int getSquareY() const {
            return m_squareY;
        }

        /**
         * Sets the integer representing the Y-compontent of the selected square
         * @param squareY An integer representing the Y-component of the selected square.
         */
        void setSquareY(int squareY) {
            m_squareY = squareY;
        }

}; // end of class NTTTMove 

#endif // _NTTPMOVE_H_

