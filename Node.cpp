#include "Node.h"
#include "Tile.h"

Node::Node()
{
}
Node::Node(Tile *tile, Node *next)
{
   this->tile = tile;
   this->next = next;
}

Node::~Node()
{
   delete tile;
}

Node::Node(Node &other) : tile(other.tile),
                          next(other.next)
{
}
