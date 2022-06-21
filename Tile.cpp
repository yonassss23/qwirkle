#include "Tile.h"
#include <iostream>

Tile::Tile(Colour c, Shape s)
{
    this->colour = c;
    this->shape = s;
    
    //appending a char and int to string
    this->fullName.push_back(c);
    this->fullName += std::to_string(s);
}
Tile::Tile(Tile &other)
{
    this->colour = other.colour;
    this->shape = other.shape;
    this->fullName = other.fullName;
}

Tile::~Tile()
{
}