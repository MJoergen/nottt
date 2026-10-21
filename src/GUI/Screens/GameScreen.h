#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_

#include "Screen.h"
#include "../Text.h"
#include <vector>
#include <atomic>
#include "../../NTTTGame.h"
#include "../../NTTTPlayer.h"

//TODO: Finish GameScreen

class GameScreen : public Screen {
public:
    struct GameData{
        ScreenState* currentState;
        NTTTGame *game;
        TTF_Font *movesFont;
        int *boardCount;
        int *boardSize;
        int *lineSize;
        int *gameSeed;
        bool *manualMode;
        bool *writeLog;
        std::string *logName;
        std::atomic<bool> *quit;

        Texture *redCross;
        Texture *blueCross;

    };
    GameScreen(GameData data);
    virtual ~GameScreen();
    virtual void init(SDL_Renderer *renderer, TTF_Font* headlineFont, TTF_Font* guiFont, const unsigned int width, const unsigned int height);
    virtual void render(SDL_Renderer* renderer) const;
    virtual void input(const SDL_Event & e);
    virtual void update(SDL_Renderer *renderer);
    virtual void prepareForQuit();
    virtual void cleanUp();
    int manageGame();

    //Modified copy from GameInfoViewer ----->
    unsigned int getMovesCount(){
        if (m_movesTexts.size() < m_maxMoves)
            return m_movesTexts.size();
        else
            return m_maxMoves + m_excessMoves;
    }
    //<-----

private:
    bool m_isThreadProperlyClosed = true;

    void initialize(SDL_Renderer *renderer);
    bool m_isInitialized;
    NTTTPlayer *m_player1, *m_player2;
    NTTTGame *m_game;
    std::atomic<unsigned int> m_winner{0};

    //The game thread changes *m_game and m_moves while the main thread draws them.
    //The game thread is the only writer, so it locks only around its changes,
    //and the main thread locks while it reads them.
    SDL_mutex *m_gameMutex = nullptr;

    //The number of moves the main thread has shown. The game thread waits for
    //it to catch up before making the next move.
    std::atomic<unsigned int> m_shownMoves{0};

    //These are only set before the game thread starts, so need no locking.
    int *m_boardCount = nullptr, *m_boardSize = nullptr, *m_lineSize = nullptr, *m_gameSeed = nullptr;
    bool *m_manualMode = nullptr, *m_writeLog = nullptr;
    std::string *m_logName = nullptr;

    std::atomic<bool> m_forward{false}, m_backward{false};

    std::atomic<bool> m_isGameThreadRunning{false};
    SDL_Thread *m_gameThread = nullptr;

    TTF_Font *m_headlineFont = nullptr, *m_guiFont = nullptr;
    unsigned int m_width, m_height;
    std::atomic<bool> *m_quit = nullptr;

    Texture *m_redCross = nullptr, *m_blueCross = nullptr;

    //Modified copy from GameInfoViewer ----->
    Text *m_player1Text = nullptr, *m_player2Text = nullptr, *m_vsText = nullptr, *m_boardCountText = nullptr, *m_boardSizeText = nullptr, *m_lineSizeText = nullptr;
    Text *m_winnerText = nullptr, *m_winnerDisplayText = nullptr;
    unsigned int m_amountInColumn, m_amountInRow;

    std::vector<Text*> m_movesTexts;

    TTF_Font *m_movesFont = nullptr;
    int m_headlineHeight, m_movesHeight, m_movesWidth;
    unsigned int m_maxMoves, m_excessMoves = 0;
    //<-----

    //Modified copy from NTTTManager ----->
    void writeLog(const int winner) const;
    const int BOARD_PADDING = 30;                   //The padding between the borders

    int m_playingFieldSize;
    int m_gridSize;
    int m_boardRenderSize;

    std::vector<NTTTMove> m_moves;
    //<-----

    void delayUnlessQuit(unsigned int ms) const;
    void initializeWinnerDisplayText(SDL_Renderer *renderer);
    void addMove(SDL_Renderer *renderer, NTTTMove move);
    void removeMove();
};

#endif