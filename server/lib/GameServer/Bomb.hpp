/*
 * Bomb.hpp
 *
 *  Created on: Feb 16, 2015
 *      Author: kodo
 */

#ifndef BOMB_HPP_
#define BOMB_HPP_

#include "GenObj.hpp"
#include <string>
#include "Game.hpp"

class Bomb : public GenObject
{
public:
	float getTimeLeft() const;
	void setTimeLeft(float);
	short int getPower() const;
	void setPower(short int);
	std::string getOwnerGuid() const;
	void setOwnerGuid(std::string);

	Bomb(std::string, int, int, int, int, Game* game);
	~Bomb();
	void stepUpdateAfterDif();
	void explode();
private:
	std::string ownerGuid;
	float timeLeft;
	short int power;
	Game* game;
};


#endif /* BOMB_HPP_ */
