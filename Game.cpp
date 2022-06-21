#include <iostream>
#include "Game.h"
#include "TileCodes.h"
#include <random>
#include <fstream>
#include <string>

Game::Game()
{
    this->bag = new TileBag();
    this->board = new Board();
    this->setBeingPlayed(false);
    this->currentPlayerID = 0;
    this->playerCount = 0;
}

Game::~Game()
{
    delete this->bag;
    delete this->board;
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        delete this->players[i];
    }
}

void Game::addPlayer(std::string name)
{
    this->playerCount++;
    this->players[this->playerCount - 1] = new Player(name);
}

Player *Game::getPlayer(unsigned int playernum)
{
    return this->players[playernum];
}

TileBag *Game::getBag()
{
    return this->bag;
}

void Game::setBag(std::string bagAsString)
{
    this->bag->clearBag();
    for (int i = 0; i < int(bagAsString.length()); i += 3)
    {
        std::string tile = bagAsString.substr(i, 2);
        char colour = tile[0];
        int shape = stoi(tile.substr(1));

        Tile *newTile = new Tile(colour, shape);
        this->bag->addTile(newTile);
    }
}

void Game::setCurrentPlayerID(int playerID)
{
    this->currentPlayerID = playerID;
}

int Game::getCurrentPlayerID()
{
    return this->currentPlayerID;
}

int Game::getPlayerCount()
{
    return this->playerCount;
}

Board *Game::getBoard()
{
    return this->board;
}

void Game::loadGame(std::string filename)
{
    if (this->fileExists(filename))
    {
        std::ifstream file(filename);
        std::string line;
        std::string fileArray[SAVE_FILE_LENGTH];
        for (int i = 0; i < SAVE_FILE_LENGTH; i++)
        {
            std::getline(file, line);
            fileArray[i] = line;
        }

        this->loadGameState(fileArray);
        this->setBeingPlayed(true);

        std::cout << "Qwirkle game successfully loaded" << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "File does not exist" << std::endl;
    }
}

void Game::loadGameState(std::string fileArray[])
{
    this->board = new Board();

    // Add player 1 content
    this->addPlayer(fileArray[0]);
    this->getPlayer(0)->setScore(stoi(fileArray[1]));
    this->getPlayer(0)->setHand(fileArray[2]);
    // Add player 2 content
    this->addPlayer(fileArray[3]);
    this->getPlayer(1)->setScore(stoi(fileArray[4]));
    this->getPlayer(1)->setHand(fileArray[5]);
    // Set Board
    this->getBoard()->setBoard(fileArray[7]);
    // Set bag
    this->setBag(fileArray[8]);
    // Set current player
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        if (this->getPlayer(i)->getName().compare(fileArray[9]) == 0)
        {
            this->setCurrentPlayerID(i);
        }
    }
}

void Game::showCredits()
{
    std::cout << "----------------------------------" << std::endl;
    this->printStudentDetails("Yonas Sisay", "s3659939", "s3659939@student.rmit.edu.au");
    this->printStudentDetails("Roman O'Brien", "s3845295", "s3845295@student.rmit.edu.au");
    this->printStudentDetails("David Morrin", "s3845230", "s3845230@student.rmit.edu.au");
    this->printStudentDetails("Lachlan Stavrevski", "s3843294", "s3843294@student.rmit.edu.au");
    std::cout << "----------------------------------" << std::endl;
    std::cout << std::endl;
}

void Game::printStudentDetails(std::string fullname, std::string stuNo, std::string email)
{
    std::cout << "Name: " << fullname << std::endl;
    std::cout << "Student ID: " << stuNo << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << std::endl;
}

void Game::setBeingPlayed(bool playState)
{
    this->beingPlayed = playState;
}

bool Game::isBeingPlayed()
{
    return this->beingPlayed;
}

void Game::showGameState()
{
    std::cout << this->getPlayer(this->getCurrentPlayerID())->getName() << ", it's your turn" << std::endl;
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        std::cout << "Score for " << this->getPlayer(i)->getName() << ": " << this->getPlayer(i)->getScore() << std::endl;
    }

    this->board->showBoard();

    std::cout << std::endl;
    std::cout << "Your hand is" << std::endl;
    this->getPlayer(this->getCurrentPlayerID())->showHand();
}

void Game::addTilesToBag()
{
    this->bag->shuffle();
}

void Game::distributeTilesToPlayers()
{
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        for (int j = 0; j < GAME_DIM; j++)
        {
            this->addTileToPlayerHand(i);
        }
    }
}

void Game::addTileToPlayerHand(int playerID)
{
    if (this->bag->size() > 0)
    {
        this->getPlayer(playerID)->addTile(this->bag->popTile());
    }
}

void Game::placeTileOnBoard(Tile *tile, int row, int col)
{
    this->board->placeTile(tile, row, col);
}

void Game::replaceTileInHand(int selectedTileIndex)
{
    Tile *oldTile = new Tile(*this->getPlayer(this->getCurrentPlayerID())->getHand()->get(selectedTileIndex));
    // TAKE TILE FROM HAND
    this->getPlayer(this->getCurrentPlayerID())->getHand()->remove(selectedTileIndex);
    // PUT TILE IN BAG
    this->bag->addTile(oldTile);
    // SHUFFLE BAG
    this->bag->shuffle();
    // GET NEW TILE
    this->addTileToPlayerHand(this->getCurrentPlayerID());
}

void Game::saveGame(std::string filename)
{
    std::ofstream outputFile(filename);
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        outputFile << this->getPlayer(i)->getName() << std::endl;
        outputFile << this->getPlayer(i)->getScore() << std::endl;
        outputFile << this->getPlayer(i)->getHandAsString() << std::endl;
    }
    outputFile << BOARD_DIM << "," << BOARD_DIM << std::endl;
    outputFile << this->getBoard()->getStateAsString() << std::endl;
    outputFile << this->getBag()->getBagAsString() << std::endl;
    outputFile << this->getPlayer(this->getCurrentPlayerID())->getName() << std::endl;

    outputFile.close();

    this->getPlayer(this->getCurrentPlayerID())->setRepeatTurn(true);
}

bool Game::fileExists(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void Game::continueGamePlay(bool &gameRunning)
{
    bool playerHandEmpty = false;
    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        if (this->getPlayer(i)->getHandAsString().compare("") == 0)
        {
            playerHandEmpty = true;
        }
    }
    if (this->getBag()->size() == 0 && playerHandEmpty)
    {
        this->showGameOverMessage();
        this->endGame(gameRunning);
    }
    else
    {
        this->switchPlayer();
    }
}

void Game::showGameOverMessage()
{
    std::cout << "Game over" << std::endl;
    int winnerIndex = -1;
    int highestScore = 0;
    bool gameDraw = false;

    for (int i = 0; i < this->getPlayerCount(); i++)
    {
        std::cout << "Score for " << this->getPlayer(i)->getName() << ": " << this->getPlayer(i)->getScore() << std::endl;
        if (highestScore < this->getPlayer(i)->getScore())
        {
            highestScore = this->getPlayer(i)->getScore();
            winnerIndex = i;
        }
        else if (highestScore == this->getPlayer(i)->getScore())
        {
            gameDraw = true;
        }
    }

    if (gameDraw)
    {
        std::cout << "Game draw!" << std::endl;
    }
    else
    {
        std::cout << "Player " << this->getPlayer(winnerIndex)->getName() << " won!" << std::endl;
    }
}

void Game::switchPlayer()
{

    // IF PLAYER IS NOT REPEATING THEIR TURN, SWITCH PLAYERS
    if (!this->getPlayer(this->getCurrentPlayerID())->getRepeatTurn())
    {
        // IF CURRENT PLAYER IS THE LAST ON LIST, MAKE NEW CURRENT THE FIRST PLAYER
        if (this->getCurrentPlayerID() == this->getPlayerCount() - 1)
        {
            this->setCurrentPlayerID(0);
        }
        // ELSE MOVE TO NEXT PLAYER
        else
        {
            this->setCurrentPlayerID(this->getCurrentPlayerID() + 1);
        }
    }
    else
    {
        this->getPlayer(this->getCurrentPlayerID())->setRepeatTurn(false);
    }
}

void Game::startGame()
{
    this->setBeingPlayed(true);

    this->addTilesToBag();
    this->distributeTilesToPlayers();
}

void Game::endGame(bool &gameRunning)
{
    std::cout << "Goodbye" << std::endl;
    std::cout << std::endl;
    this->setBeingPlayed(false);
    gameRunning = false;
}