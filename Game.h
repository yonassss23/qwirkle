
#ifndef ASSIGN2_GAME_H
#define ASSIGN2_GAME_H

#include "Player.h"
#include "LinkedList.h"
#include "Board.h"
#include "TileBag.h"
#include <vector>

#define MAX_NUM_OF_PLAYERS 2
#define MAX_NUM_OF_TILES 72
#define SAVE_FILE_LENGTH 10
#define GAME_DIM 6

class Game
{
public:
    Game();
    ~Game();

    TileBag *getBag();
    Board *getBoard();

    // STARTING GAME
    void startGame();
    void distributeTilesToPlayers();
    void addPlayer(std::string name);

    // SAVING GAME TO SAVEFILE
    void saveGame(std::string filename);

    // LOADING GAME FROM SAVEFILE
    void loadGame(std::string filename);
    void loadGameState(std::string fileArray[]);
    void setBeingPlayed(bool playState);

    void setBag(std::string bagAsString);

    // GAME STATUS
    bool isBeingPlayed();
    void showGameState();
    void continueGamePlay(bool &gameRunning);


    // FILE STATUS
    bool fileExists(std::string filename);

    // PLAYER FUNCTIONS
    Player *getPlayer(unsigned int playernum);
    int getPlayerCount();
    int getCurrentPlayerID();
    void setCurrentPlayerID(int playerID);
    void switchPlayer();

    // TILE FUNCTIONS
    void addTileToPlayerHand(int playerID);
    void placeTileOnBoard(Tile* tile, int row, int col);
    void replaceTileInHand(int selectedTileIndex);
    void addTilesToBag();

    // PRINT FUNCTIONS
    void showCredits();
    void showGameOverMessage();
    void printStudentDetails(std::string fullname, std::string stuNo, std::string email);

    // END GAME
    void endGame(bool &gameRunning);

private:
    Player *players[MAX_NUM_OF_PLAYERS];
    TileBag *bag;
    Board *board;

    int currentPlayerID;
    int playerCount;

    bool beingPlayed;

};

#endif // ASSIGN2_GAME_H
