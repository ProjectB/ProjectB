/*
 * Bomb.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: kodo
 */

#include "Bomb.hpp"
#include "../defs.hpp"
#include "GameState.hpp"

#include <algorithm>

Bomb::Bomb(std::string guid, int x, int y, int w, int h)
: GenObject(guid, ObjType::_Bomb, x, y, w, h)
{
	this->timeLeft = BOMB_LIFETIME;
	this->power = 1;
}
Bomb::~Bomb()
{
	GameState gs;
	gs.onBombExplosion(this);
	delete this;
}

/* GETTERS && SETTERS */
short int Bomb::getTimeLeft() const
{
	return this->timeLeft;
}
void Bomb::setTimeLeft(short int timeLeft)
{
	this->timeLeft = timeLeft;
}
short int Bomb::getPower() const
{
	return this->power;
}
void Bomb::setPower(short int power)
{
	this->power = std::max((short int)BOMB_MIN_POWER, power);
}
std::string Bomb::getOwnerGuid() const
{
	return this->ownerGuid;
}
void Bomb::setOwnerGuid(std::string ownerGuid)
{
	this->ownerGuid = ownerGuid;
}
/* GETTERS && SETTERS */
