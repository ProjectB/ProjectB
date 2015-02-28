/*
 * Bomb.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: kodo
 */

#include "Bomb.hpp"
#include "../defs.hpp"

#include <algorithm>

Bomb::Bomb(std::string guid, int x, int y, int w, int h, Game* game)
: GenObject(guid, ObjType::_Bomb, x, y, w, h)
{
	this->game = game;
	this->timeLeft = BOMB_LIFETIME;
	this->power = 1;
}
Bomb::~Bomb()
{
	std::cout << "bomb exploded" << std::endl;
	this->game->onBombExplosion(this);
	std::cout << "after bomb explosion.. object should disappear" << std::endl;
}
void Bomb::update()
{
	this->timeLeft -= FRAME_TIME;
	if(this->timeLeft <= 0)
		delete this;
}
/* GETTERS && SETTERS */
float Bomb::getTimeLeft() const
{
	return this->timeLeft;
}
void Bomb::setTimeLeft(float timeLeft)
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
