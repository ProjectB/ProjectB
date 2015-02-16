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

class Bomb : public GenObject
{
public:
	short int getTimeLeft() const;
	void setTimeLeft(short int);
	short int getPower() const;
	void setPower(short int);
	std::string getOwnerGuid() const;
	void setOwnerGuid(std::string);

	Bomb(std::string, int, int, int, int);
	~Bomb();
private:
	std::string ownerGuid;
	short int timeLeft;
	short int power;
};


#endif /* BOMB_HPP_ */
