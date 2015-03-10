/*
 * Game.cpp
 *
 *  Created on: Feb 22, 2015
 *      Author: kodo
 */

#include "../defs.hpp"
#include "GameServer.hpp"
#include "Block.hpp"
#include "Bomber.hpp"
#include "Bomb.hpp"

using namespace std;

Game::Game()
{
	this->gs = new GameState();
	this->nextGuid = 0;

	for(int i=1; i < 11; i+=2)
	{
		for(int j=1; j < 11; j+=2)
		{
			this->newObject(BLOCK_WIDTH * i, BLOCK_HEIGHT * j, ObjType::_Square);
		}
	}

	this->isRunning = true;
	this->stepThread = thread([this] {this->step();});
}
Game::~Game()
{
	this->isRunning = false;
    this->stepThread.join();
	this->recentlyCreatedObjects.clear();
	this->objects.clear();
	this->players.clear();
}
void Game::step()
{
	timeval begin, end;
	double elapsedTime;
	unsigned int million = 1000000;
	int sleepTime;

	while(this->isRunning)
	{
		gettimeofday(&begin, NULL);

		//TODO: sleep should be for a game state... things are confusing here!
		if(this->connectedPlayersGuid.size() > 0)
		{
			string msg = this->gs->generateDifStateMessage(&this->objects, &this->recentlyCreatedObjects, &this->objectsToBeDeleted);

			if (!msg.empty()) {
				GameServer::getInstance().sendMessageToClientArray(this->getAllGuids(), msg);
			}
		}
		gettimeofday(&end, NULL);

		elapsedTime = ((end.tv_sec * million +  end.tv_usec) - (begin.tv_sec * million + begin.tv_usec)) / 1000;
		sleepTime = (int)((double)(1000/FPS) - elapsedTime);

		this_thread::sleep_for(chrono::milliseconds(sleepTime));
	}
}
GenObject* Game::newObject(int x, int y, ObjType type)
{
	std::stringstream guid;
	guid << (this->nextGuid++);

	int w, h;

	GenObject *obj;

	switch(type)
	{
		case ObjType::_Square:
			w = BLOCK_WIDTH;
			h = BLOCK_HEIGHT;
			obj = new Block(false, guid.str(), x, y, w, h);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(guid.str());
			break;
		case ObjType::_Player:
			w = BOMBER_WIDTH;
			h = BOMBER_HEIGHT;
			obj = new Bomber(guid.str(), x, y, w, h, this);
			this->recentlyCreatedObjects.push_back(obj);
			this->players.push_back(guid.str());
			break;
		case ObjType::_Bomber:
			w = BOMBER_WIDTH;
			h = BOMBER_HEIGHT;
			obj = new Bomber(guid.str(), x, y, w, h, this);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(guid.str());
			break;
		case ObjType::_Bomb:
			w = BOMB_WIDTH;
			h = BOMB_HEIGHT;
			obj = new Bomb(guid.str(), x, y, w, h, this);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(guid.str());
			break;
		default:
			break;
	}


	if (_GAMESTATE_DEBUG == 1) {
		std::cout << "OBJECT CREATED[" << type << "]: guid=" << guid << ": (" << x << "," << y << ")";
		std::cout << " (" << x+w << "," << y << ")";
		std::cout << " (" << x+w << "," << y+h << ")";
		std::cout << " (" << x << "," << y+h << ")" << std::endl;
		//printf("OBJECT CREATED: number %d: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", n-1, x, y, x+w, y, x+w, y+h, x, y+h);
	}

	return obj;
}
void Game::deleteObject(unsigned int i)
{
	this->objectsToBeDeleted.push_back(this->objects[this->npObjects[i]]);
	this->objects.erase(this->npObjects[i]);
	this->npObjects.erase(this->npObjects.begin() + i);
}
void Game::deleteObject(std::string guid)
{
	for(unsigned int i = 0; i < this->npObjects.size(); i++)
	{
		if(this->npObjects[i] == guid)
			this->deleteObject(i);
	}
}
GenObject* Game::createPlayer(int x, int y)
{
	GenObject *player = this->newObject(x, y, ObjType::_Player);
	this->connectedPlayersGuid.push_back(player->guid);
	this->objects[player->guid] = player;

	return player;
}
std::string Game::deletePlayer(std::string guid)
{
	std::string msg = this->objects[guid]->generateObjectActionMessage((ObjectAction)Delete);
	this->objects.erase(guid);
	this->removeConnectedPlayerGuid(guid);

	return msg;
}
void Game::removeConnectedPlayerGuid(std::string guid)
{
	for(unsigned int i=0; i < this->connectedPlayersGuid.size(); i++)
	{
		if(this->connectedPlayersGuid[i] == guid)
		{
			this->connectedPlayersGuid.erase(this->connectedPlayersGuid.begin() + i);
			return;
		}
	}
}
void Game::update(std::string guid, std::string msg)
{
	if(this->objects.find(guid) != this->objects.end())
	{
		this->objects[guid]->update(msg);
	}
}
void Game::bomberDropsABomb(std::string guid)
{
	if(this->objects.find(guid) != this->objects.end() && this->objects[guid]->type == ObjType::_Bomber)
	{
		Bomber *bomber = (Bomber*)this->objects[guid];
		Bomb *bomb = (Bomb*)this->newObject(bomber->x, bomber->y, ObjType::_Bomb);
		bomb->setOwnerGuid(guid);
		bomb->setPower(bomber->getBombStr());
	}
}
void Game::onBombExplosion(void* bomb)
{
	Bomb* b = (Bomb*)bomb;
	this->deleteObject(b->guid);
	std::string ownerGuid = b->getOwnerGuid();
	if(this->objects.find(ownerGuid) != this->objects.end() && this->objects[ownerGuid]->type == ObjType::_Bomber)
	{
		Bomber *bombOwner = (Bomber*)this->objects[ownerGuid];
		bombOwner->bombExploded();
	}
}
std::vector<std::string> Game::getAttributes(unsigned int offset, std::string msg) const
{
	std::vector<std::string> attributes;
	unsigned int i=offset;
	std::string str = "";

	while(msg[i++] != ':');

	for(;i < msg.size(); i++)
	{
		while(msg[i] != ';' && i < msg.size())
			str.push_back(msg[i++]);
		attributes.push_back(str);
		str.clear();
	}

	return attributes;
}
std::string Game::getDifStateMessage()
{
	return this->gs->generateDifStateMessage(&this->objects, &this->recentlyCreatedObjects, &this->objectsToBeDeleted);
}
unsigned int Game::getNumPlayersOnGame() const
{
	return this->connectedPlayersGuid.size();
}
std::vector<std::string> Game::getAllGuids() const
{
	return this->connectedPlayersGuid;
}
