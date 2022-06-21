#ifndef ASSIGN2_TILEBAG_H
#define ASSIGN2_TILEBAG_H

#include "LinkedList.h"
#include "Tile.h"

class TileBag
{
public:
    TileBag();
    ~TileBag();
    int size();
    void shuffle();
    Tile *popTile();
    void addTile(Tile *data);
    std::string getBagAsString();
    void clearBag();

private:
    Tile *topTile;
    LinkedList *tiles;
};

#endif // ASSIGN2_TILEBAG_H