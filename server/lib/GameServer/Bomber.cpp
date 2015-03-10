/*
 * Bomber.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#include "Bomber.hpp"
#include "GameState.hpp"
#include "../util/UtilMethods.hpp"
#include "../defs.hpp"


using namespace std;
#include <string>
#include <sstream>
#include <vector>

Bomber::Bomber(std::string guid, int x, int y, int w, int h, Game* game)
: GenObject(guid, ObjType::_Bomber, x, y, w, h)
{
	this->game = game;
	this->HP = 1;
	this->bombStr = 1;
	this->bombLimit = 1;
	this->numBombsDropped = 0;
	this->lastDirection = EnumMoveDirection::Down;
}

Bomber::~Bomber()
{

}

/* PRIVATE FUNCTIONS */
std::string Bomber::generateObjectState() const
{
	std::stringstream ss;

	ss << this->HP;
	ss << ";" << this->bombStr;
	ss <<  ";" << this->bombLimit;

	return ss.str();
}
void Bomber::dropBomb()
{
	if(this->numBombsDropped < this->bombLimit)
	{
		this->game->bomberDropsABomb(this->guid);
	}
}
void Bomber::bombExploded()
{
	this->numBombsDropped--;
}
/* PRIVATE FUNCTIONS */
/* PUBLIC FUNCTIONS */

void Bomber::update(std::string msg)
{
	int xdif = 0, ydif = 0;
	std::vector<std::string> input = UtilMethods::splitInputMessage(msg);

	for(std::vector<std::string>::const_iterator it = input.begin(); it != input.end(); it++)
	{
		std::string topic = UtilMethods::getMessageTopic(*it);
		if (topic.compare("moveLeft") == 0)
		{
			this->lastDirection = EnumMoveDirection::Left;
			xdif = -PLAYER_X_MOVE;
		}
		else if (topic.compare("moveRight") == 0)
		{
			this->lastDirection = EnumMoveDirection::Right;
			xdif = PLAYER_X_MOVE;
		}
		else if (topic.compare("moveUp") == 0)
		{
			this->lastDirection = EnumMoveDirection::Up;
			ydif = -PLAYER_Y_MOVE;
		}
		else if (topic.compare("moveDown") == 0)
		{
			this->lastDirection = EnumMoveDirection::Down;
			ydif = PLAYER_Y_MOVE;
		}
		else if (topic.compare("dropBomb") == 0)
		{
			this->dropBomb();
		}
		else {
			std::cout << "WARNING: UNKNOWN BOMBER TOPIC @guid " << this->guid << " >> " << topic << std::endl;
		}
	}

	if(xdif != 0 || ydif != 0)
		this->Move(xdif, ydif);
}
std::string Bomber::generateObjectActionMessage(ObjectAction act) const
{
	std::stringstream ss;
	ss << GenObject::generateObjectActionMessage(act);

	switch(act)
	{
	case Add:
		break;
	case Update:
		ss << ";" << this->lastDirection << ";" << this->getHasMovedThisFrame();
		break;
	case Delete:
		break;
	default:
		break;
	}

	return ss.str();
}

/* PUBLIC FUNCTIONS */

/* GETTERS && SETTERS */
short int Bomber::getBombStr() const {
	return this->bombStr;
}
void Bomber::setBombStr(short int bombStr) {
	this->bombStr = bombStr;
}

short int Bomber::getHP() const {
	return this->HP;
}
void Bomber::setHP(short int HP) {
	this->HP = HP;
}
short int Bomber::getBombLimit() const {
	return bombLimit;
}
void Bomber::setBombLimit(short int bombLimit) {
	this->bombLimit = bombLimit;
}
void Bomber::increaseBombStr(short int val)
{
	this->bombStr += val;
}
void Bomber::decreaseBombStr(short int val)
{
	this->bombStr -= val;
}
void Bomber::increaseBombLimit(short int val)
{
	this->bombLimit += val;
}
void Bomber::decreaseBombLimit(short int val)
{
	this->bombLimit -= val;
}
void Bomber::increaseHP(short int val)
{
	this->HP += val;
}
void Bomber::decreaseHP(short int val)
{
	this->HP -= val;
	if(this->HP <= 0)
		delete this;

}
std::string Bomber::getObjectState() const
{
	return this->generateObjectState();
}
/* GETTERS && SETTERS */
