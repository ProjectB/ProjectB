/*
 * Bomber.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#include "Bomber.hpp"
#include "GameState.hpp"
#include "../util/UtilMethods.hpp"


using namespace std;
#include <string>
#include <sstream>
#include <vector>

Bomber::Bomber(std::string guid, int x, int y, int w, int h)
: GenObject(guid, ObjType::_Bomber, x, y, w, h)
{
	this->HP = 1;
	this->bombStr = 1;
	this->bombLimit = 1;
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
void Bomber::dropBomb() const
{
	std::cout << "Bomber " << this->guid << " dropping a bomb." << std::endl;
	GameState gs;
	gs.bomberDropsABomb(this->guid);
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
			xdif = -PLAYER_X_MOVE;
		else if (topic.compare("moveRight") == 0)
			xdif = PLAYER_X_MOVE;
		else if (topic.compare("moveUp") == 0)
			ydif = -PLAYER_Y_MOVE;
		else if (topic.compare("moveDown") == 0)
			ydif = PLAYER_Y_MOVE;
		else if (topic.compare("dropBomb") == 0)
		{
			this->dropBomb();
			//std::vector<std::string> attr = this->getAttributes(topic.size(), *it);
			//this->newNPObject(atoi(attr[0].c_str()), atoi(attr[1].c_str()), ObjType::_Bomb);
		}
		else {
			std::cout << "WARNING: UNKNOWN BOMBER TOPIC @guid " << this->guid << " >> " << topic << std::endl;
		}
	}

	if(xdif != 0 || ydif != 0)
		this->Move(xdif, ydif);
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
