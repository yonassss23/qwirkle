#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>
#include "Tile.h"
#include "LinkedList.h"

class Player
{
public:
    Player(std::string name);
    ~Player();

    std::string getName();
    int getScore();
    LinkedList *getHand();

    void addTile(Tile *tile);

    void setScore(int score);
    void addScore(int score);

    bool getRepeatTurn();
    void setRepeatTurn(bool repeatTurn);

    void showHand();
    void setHand(std::string handAsString);
    std::string getHandAsString();

private:
    std::string name;
    int score;
    bool repeatTurn;
    LinkedList *hand;
};

#endif // ASSIGN2_PLAYER_H