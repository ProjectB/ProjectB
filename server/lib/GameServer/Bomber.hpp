/*
 * Bomber.hpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#ifndef BOMBER_HPP_
#define BOMBER_HPP_

#include "GenObj.hpp"
using namespace std;
#include <string>

class Bomber : public GenObject
{
public:
	/* GETTERS/SETTERS/UPDATES */
	short int getBombStr() const;
	void setBombStr(short int);
	short int getHP() const;
	void setHP(short int);
	short int getBombLimit() const;
	void setBombLimit(short int);
	void increaseBombStr(short int);
	void decreaseBombStr(short int);
	void increaseBombLimit(short int);
	void decreaseBombLimit(short int);
	void increaseHP(short int);
	void decreaseHP(short int);

	std::string getObjectState() const;
	/* GETTERS/SETTERS/UPDATES */
	Bomber(std::string, int, int, int, int);
	~Bomber();
	void update(std::string);
private:
	short int HP; //Hit Points
	short int bombStr; //bomb strength, in squares
	short int bombLimit; //number of concurrent bombs that can be placed

	std::string generateObjectState() const;
	void dropBomb() const;
};


#endif /* BOMBER_HPP_ */
