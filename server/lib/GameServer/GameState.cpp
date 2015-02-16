/*
 * GameState.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#include "GameState.hpp"

using namespace std;

GameState::GameState()
{
	this->n = 0;
	this->height = SCREEN_HEIGHT;
	this->width = SCREEN_WIDTH;
	this->xMove = 4;
	this->yMove = 4;

	for(int i=1; i < 11; i+=2)
	{
		for(int j=1; j < 11; j+=2)
		{
			this->newObject(i, j, ObjType::_Square);
		}
	}
}
GameState::~GameState()
{
	this->recentlyCreatedObjects.clear();
	this->objects.clear();
	this->players.clear();
}
GenObject* GameState::newObject(int posX, int posY, ObjType type)
{
	std::stringstream ss;
	ss << (this->n++);

	int x, y, w, h;

	x = BLOCK_WIDTH * posX;
	y = BLOCK_HEIGHT * posY;
	GenObject *obj;

	switch(type)
	{
		case ObjType::_Square:
			w = BLOCK_WIDTH;
			h = BLOCK_HEIGHT;
			obj = new Block(false, ss.str(), x, y, w, h);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(ss.str());
			break;
		case ObjType::_Player:
			w = BOMBER_WIDTH;
			h = BOMBER_HEIGHT;
			obj = new Bomber(ss.str(), x, y, w, h);
			this->recentlyCreatedObjects.push_back(obj);
			this->players.push_back(ss.str());
			break;
		case ObjType::_Bomber:
			w = BOMBER_WIDTH;
			h = BOMBER_HEIGHT;
			obj = new Bomber(ss.str(), x, y, w, h);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(ss.str());
			break;
		case ObjType::_Bomb:
			w = BOMB_WIDTH;
			h = BOMB_HEIGHT;
			obj = new Bomb(ss.str(), x, y, w, h);
			this->recentlyCreatedObjects.push_back(obj);
			this->npObjects.push_back(ss.str());
			break;
		default:
			break;
	}


	if (_GAMESTATE_DEBUG == 1) {
		std::cout << "OBJECT CREATED[" << type << "]: n=" << this->n-1 << ": (" << x << "," << y << ")";
		std::cout << " (" << x+w << "," << y << ")";
		std::cout << " (" << x+w << "," << y+h << ")";
		std::cout << " (" << x << "," << y+h << ")" << std::endl;
		//printf("OBJECT CREATED: number %d: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", n-1, x, y, x+w, y, x+w, y+h, x, y+h);
	}

	return obj;
}
void GameState::deleteObject(unsigned int i) {
	this->objects.erase(this->npObjects[i]);
	this->npObjects.erase(this->npObjects.begin() + i);
}
std::string GameState::updateNPObjects()
{
	std::stringstream ss;
	GenObject *object;
	for(unsigned int i = 0; i < this->npObjects.size(); i++)
	{
		object = this->objects[this->npObjects[i]];
		switch(object->type)
		{
			case ObjType::_Bomb:
				if(--object->timeInFrames == 0) {
					ss << object->generateObjectActionMessage((ObjectAction)Delete) << SEPARATOR;
					this->deleteObject(i);
				}
				break;
			default:
				break;
		}
	}

	return ss.str();
}
GenObject* GameState::createPlayer(int x, int y) {
	return this->newObject(x, y, ObjType::_Player);
}
std::string GameState::generateDifStateMessage() {
	std::string str;

	//TODO: LOCK fixedOBjects + players WHILE DOING DIFF STATE
	str += this->generateObjectsDifStateMessage();
	if (_GAMESTATE_DEBUG == 1)
		if(!str.empty())
			std::cout << "DIF STATE >> " << str << std::endl;

	return str;
}
std::string GameState::generateObjectsDifStateMessage()
{
	std::stringstream ss;
	unsigned int i=0;

	for (std::map<std::string, GenObject*>::iterator it = this->objects.begin(); it != this->objects.end(); it++)
	{
		GenObject *obj = (*it).second;
		if(obj->hasChanges)
		{
			ss << obj->generateObjectActionMessage((ObjectAction)Update) << SEPARATOR;
			obj->hasChanges = false;
		}
	}

	for(i=0; i < this->recentlyCreatedObjects.size(); i++)
	{
		GenObject *obj = this->recentlyCreatedObjects[i];
		ss << obj->generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
		this->objects[obj->guid] = obj;
	}
	this->recentlyCreatedObjects.clear();

	return ss.str();
}
void GameState::update(std::string guid, std::string msg)
{
	if(this->objects.find(guid) != this->objects.end())
	{
		this->objects[guid]->update(msg);
	}
}
void GameState::bomberDropsABomb(std::string guid)
{
	std::cout << "Bomber " << guid << " wants to drop a bomb." << std::endl;
	std::cout << "size = " << this->objects.size() << std::endl;
	if(this->objects.find(guid) != this->objects.end() && this->objects[guid]->type == ObjType::_Bomber)
	{
		std::cout << "it exists." << std::endl;
		Bomber *bomber = (Bomber*)this->objects[guid];
		std::cout << "creating bomb " << std::endl;
		Bomb *bomb = (Bomb*)this->newObject(bomber->x, bomber->y, ObjType::_Bomb);
		std::cout << "bomb created " << std::endl;
		bomb->setOwnerGuid(guid);
		bomb->setPower(bomber->getBombStr());
		std::cout << "stats setted" << std::endl;
	}
	else
	{
		if(this->objects.find(guid) != this->objects.end())
		{
			std::cout << "at least it exists.. but its type is " << this->objects[guid]->type << std::endl;
		}
		else
		{
			std::cout << "no ecziste" << std::endl;
		}
	}
}
void GameState::onBombExplosion(Bomb* bomb)
{

}
std::vector<std::string> GameState::getAttributes(unsigned int offset, std::string msg) const
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
