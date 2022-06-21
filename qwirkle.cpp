
#include "LinkedList.h"
#include "TileBag.h"
#include <iostream>
#include "Game.h"
#include "TileBag.h"
#include "Board.h"

#define EXIT_SUCCESS 0
#define DELIMITER std::string(" ")
#define CHARINTVAL 65
#define HANDSIZE 6
#define BOARD_DIM 26
#define TURNONEBAGSIZE 60

void welcomeMessage();
void showMenu();
void getInput(std::string &input);
void makeSelection(std::string, Game *&game, bool &gameRunning);

int main(void)
{
   Game *game = new Game();
   bool gameRunning = true;
   welcomeMessage();
   while (gameRunning)
   {
      showMenu();
      std::string input;
      getInput(input);
      makeSelection(input, game, gameRunning);

      while (game->isBeingPlayed())
      {
         game->showGameState();
         getInput(input);
         makeSelection(input, game, gameRunning);
         // continue game play
         game->continueGamePlay(gameRunning);
      }
   }

   delete game;
   return EXIT_SUCCESS;
}

void welcomeMessage()
{
   std::cout << std::endl;
   std::cout << "Welcome to Qwirkle!" << std::endl;
   std::cout << "-------------------" << std::endl;
}

void showMenu()
{
   std::cout << std::endl;
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
}

void getInput(std::string &input)
{
   std::cout << "> ";
   std::getline(std::cin, input);
   std::cout << std::endl;
}

void validateName(std::string &input)
{
   bool valid = false;
   while (!valid)
   {
      for (int i = 0; i < int(input.length()); i++)
      {
         if (!isalpha(input[i]))
         {
            std::cout << "The name you've input is not valid, please try again: " << std::endl;
            getInput(input);
            validateName(input);
         }
         input[i] = toupper(input[i]);
      }
      valid = true;
   }
}

void makeSelection(std::string input, Game *&game, bool &gameRunning)
{
   // NEW GAME
   if (input.compare("1") == 0 && !game->isBeingPlayed())
   {
      std::string inputName;
      std::cout << "Starting a New Game" << std::endl
                << std::endl;
      ;
      std::cout << std::endl;

      // PLAYER 1 NAME INPUT
      std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
      getInput(inputName);
      validateName(inputName);
      game->addPlayer(inputName);
      std::cout << std::endl;

      // PLAYER 2 NAME INPUT
      std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
      getInput(inputName);
      validateName(inputName);
      game->addPlayer(inputName);
      std::cout << std::endl;

      // WELCOME MESSAGE
      std::cout << "Let's Play!" << std::endl;
      std::cout << std::endl;

      // START GAME
      game->startGame();
   }
   // LOAD GAME
   else if (input.compare("2") == 0)
   {
      std::cout << std::endl;
      std::string filename;
      std::cout << "Enter the filename from which to load a game: " << std::endl;
      getInput(filename);
      game->loadGame(filename);
   }

   // REPLACE TILE
   else if (input.find("replace") != std::string::npos && input.size() == 10)
   {
      // STOP USER FROM MAKING A WORTHLESS REPLACEMENT
      if (game->getBag()->size() == 0)
      {
         std::cout << "Bag is empty. Please try again: " << std::endl;
         std::cout << std::endl;
         if (game->getPlayerCount() != 0)
         {
            game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
         }
      }
      else
      {
         Player *player = game->getPlayer(game->getCurrentPlayerID());
         int tileIndex = -1;
         std::string errorMessage = "ERROR: ";
         // IMPORT VARIABLES AS STRINGS
         input = input.erase(0, input.find(DELIMITER) + DELIMITER.length());

         // TILESTRING
         std::string tileString = input.substr(0, input.find(DELIMITER));
         input = input.erase(0, input.find(DELIMITER) + DELIMITER.length());

         // MATCH TILE INDEX AND GRAB TILE TO REPLACE
         tileIndex = player->getHand()->getIndex(tileString);
         if (0 <= tileIndex && tileIndex < HANDSIZE)
         {
            game->replaceTileInHand(tileIndex);
         }
         else
         {
            // ELSE REPEAT THE TURN AND PRINT ERROR MESSAGE
            errorMessage += "The specified tile does not exist in your hand. ";
            errorMessage += "Please try again: ";
            std::cout << errorMessage << std::endl;
            std::cout << std::endl;
            if (game->getPlayerCount() != 0)
            {
               game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
            }
         }
      }
   }

   // PLACE TILE
   else if (input.find("place") != std::string::npos && input.find("at") != std::string::npos && input.size() > 13 && std::string::npos && input.size() < 16)
   {

      int score = 0;
      Player *player = game->getPlayer(game->getCurrentPlayerID());

      Tile *tile = new Tile('Z', -1);
      int tileIndex = -1;
      int row = -1;
      int col = -1;

      // INPUT SANITSATION
      bool tileSanity = false;
      bool rowSanity = false;
      bool colSanity = false;
      std::string errorMessage = "ERROR: ";

      // IMPORT VARIABLES AS STRINGS
      input = input.erase(0, input.find(DELIMITER) + DELIMITER.length());

      // TILESTRING
      std::string tileString = input.substr(0, input.find(DELIMITER));
      input = input.erase(0, input.find(DELIMITER) + DELIMITER.length());
      input = input.erase(0, input.find(DELIMITER) + DELIMITER.length());

      //LOCATION STRING
      std::string locationString = input;

      //CHECK TILE AND LOCATION IS CORRECT TYPES
      if (isalpha(locationString[0]) && isalpha(tileString[0]) && isdigit(locationString[1]) && isdigit(tileString[1]) && isdigit(locationString[locationString.size() - 1]))
      {

         // MATCH TILE INDEX AND GRAB TILE TO ADD TO BOARD
         tileIndex = player->getHand()->getIndex(tileString);
         // CHECK IF TILE IS VALID
         if (0 <= tileIndex && tileIndex < HANDSIZE)
         {
            tileSanity = true;
            // std::cout << "tileSanity: " << tileSanity << std::endl;
            tile = player->getHand()->get(tileIndex);
         }
         else
         {
            errorMessage += "The specified tile does not exist in your hand. ";
         }

         // IMPORT ROW
         row = locationString[0] - CHARINTVAL;
         // CHECK IF ROW VALUE IS VALID
         if (0 <= row && row < BOARD_DIM)
         {
            rowSanity = true;
         }
         else
         {
            errorMessage += "The specified row is out of bounds. ";
         }

         // IMPORT COL
         std::string colString = locationString.substr(1, std::string::npos);
         col = std::stoi(locationString.substr(1, std::string::npos));
         // CHECK IF COLUMN VALUE IS VALID
         if (0 <= col && col < BOARD_DIM)
         {
            colSanity = true;
         }
         else
         {
            errorMessage += "The specified column is out of bounds. ";
         }

         // DISPLAY ERROR IF ANY INPUT IS INCORRECT
         if ((!tileSanity || !rowSanity || !colSanity))
         {
            errorMessage += "Please try again: ";
            std::cout << errorMessage << std::endl;
            std::cout << std::endl;

            if (game->getPlayerCount() != 0)
            {
               game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
            }
         }

         // LET PLAYER PLACE TILE ON TURN ONE, DON'T VALIDATE (score = 1)
         else if (game->getBag()->size() == TURNONEBAGSIZE)
         {
            score++;
         }
         // IF NOT TURN ONE, SCORE + VALIDATE TILE PLACEMENT
         else if (game->getBoard()->scoreValidate(row, col, tile) > 0)
         {
            score = game->getBoard()->scoreValidate(row, col, tile);
         }
         // TILE PLACEMENT NOT VALID
         else
         {
            std::cout << "The specified location is not valid, please try again: " << std::endl;
            std::cout << std::endl;
            if (game->getPlayerCount() != 0)
            {
               game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
            }
         }
         // IF VALID, PLACE TILE
         if (score > 0)
         {
            // CHECK FOR QWIRKLE
            if (score >= 12 && score < 24)
            {
               std::cout << "QWIRKLE!!!" << std::endl;
            }
            // DOUBLE QWIRKLE
            else if (score >= 24)
            {
               std::cout << "DOUBLEQWIRKLE!!!" << std::endl;
            }

            // PLACE TILE ON BOARD
            game->placeTileOnBoard(tile, row, col);

            // ADJUST PLAYER SCORE + HAND
            player->addScore(score);
            player->getHand()->remove(tileIndex);
            game->addTileToPlayerHand(game->getCurrentPlayerID());
         }
      }
      // INVALID INPUT
      else
      {
         std::cout << "Invalid input" << std::endl;
         std::cout << std::endl;
         if (game->getPlayerCount() != 0)
         {
            game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
         }
      }
   }

   // LOAD GAME
   else if (input.compare("2") == 0 && !game->isBeingPlayed())
   {
      std::cout << "Load Game selected" << std::endl;
   }

   // CREDITS
   // shows the credits 3 has been entered when the game is not being played
   else if (input.compare("3") == 0 && !game->isBeingPlayed())
   {
      game->showCredits();
   }

   // END GAME
   // ends the program if 4 has been entered when the game is not being played
   else if (input.compare("4") == 0 && !game->isBeingPlayed())
   {
      game->endGame(gameRunning);
   }
   // ends the whole program if the eof character has been pressed
   else if (std::cin.eof())
   {
      game->endGame(gameRunning);
   }
   else if (input.compare("quit") == 0 && game->isBeingPlayed())
   {
      game->endGame(gameRunning);
   }

   // SAVE GAME
   else if (input.find("save") != std::string::npos)
   {
      std::string filename = input.substr(5);

      game->saveGame(filename);

      std::cout << "Game saved successfully" << std::endl;
   }

   // INVALID INPUT
   else
   {
      std::cout << "Invalid input" << std::endl;
      std::cout << std::endl;
      if (game->getPlayerCount() != 0)
      {
         game->getPlayer(game->getCurrentPlayerID())->setRepeatTurn(true);
      }
   }
}
