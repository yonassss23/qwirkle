#include <stdlib.h>
#include <random>

#include "Game.h"
#include "TileCodes.h"
#include "TileBag.h"

// number of possible values each attribute of the Tile can have
#define TILE_VARIATION 6

TileBag::TileBag()
{
    // INITALISE STANDARD SET OF TILES
    this->tiles = new LinkedList();
    char colours[TILE_VARIATION] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
    int shapes[TILE_VARIATION] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

    // ADD TWO OF EACH TILE TO BAG
    for (int i = 0; i < TILE_VARIATION; i++)
    {
        for (int j = 0; j < TILE_VARIATION; j++)
        {
            this->tiles->add_back(new Tile(colours[i], shapes[j]));
            this->tiles->add_back(new Tile(colours[i], shapes[j]));
        }
    }
}

TileBag::~TileBag()
{
    delete topTile;
    delete tiles;
}

void TileBag::shuffle()
{

    // CREATE NEW LINKEDLIST
    LinkedList *shuffleBag = new LinkedList();

    // RANDOMISE TILE SELECTION USING TIME AS SEED
    while (this->tiles->size() > 0)
    {
        std::random_device engine;
        std::uniform_int_distribution<int> uniform_dist(0, this->tiles->size() - 1);

        int randomIndex = uniform_dist(engine);
        shuffleBag->add_back(this->tiles->get(randomIndex));
        this->tiles->remove(randomIndex);
    }

    // EMPTY TileBag.tiles
    this->tiles->clear();
    // MAKE TileBag.tiles EQUAL TO shuffleBag
    this->tiles = new LinkedList(*shuffleBag);

    delete shuffleBag;
}

Tile *TileBag::popTile()
{
    this->topTile = new Tile(*this->tiles->get(0));
    this->tiles->remove_front();

    return this->topTile;
}

int TileBag::size()
{
    return this->tiles->size();
}

void TileBag::addTile(Tile *data)
{
    tiles->add_back(data);
}

std::string TileBag::getBagAsString()
{
    std::string returnString = "";
    for (int i = 0; i < this->size(); i++)
    {
        returnString += this->tiles->get(i)->fullName;
        if (i < this->size() - 1)
        {
            returnString += ",";
        }
    }

    return returnString;
}

void TileBag::clearBag()
{
    tiles->clear();
}