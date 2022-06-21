
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList
{
public:
   LinkedList();
   LinkedList(LinkedList &other);
   ~LinkedList();

   int size();

   Tile *get(int index);

   // RETURNS INDEX OF TILE POINTER
   int getIndex(std::string tileString);

   std::string getContentAsString();
   void add_front(Tile *data);
   void add_back(Tile *data);

   // Contract: Elements should exist in the list to be deleted.
   void remove_front();
   void remove_back();

   void remove(int index);

   void clear();

private:
   Node *head;
};

#endif // ASSIGN2_LINKEDLIST_H
