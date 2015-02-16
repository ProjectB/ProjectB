/*
 * Block.hpp
 *
 *  Created on: Jan 25, 2015
 *      Author: kodo
 */

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "GenObj.hpp"
using namespace std;
#include <string>

class Block : public GenObject
{
public:
	/* GETTERS && SETTERS */
	short int getHP() const;
	void setHP(short int);
	void decreaseHP(short int);
	std::string getObjectState() const;
	/* GETTERS && SETTERS */

	Block(bool, std::string, int, int, int, int);
	~Block();
private:
	short int HP; //Hit Points
	bool isBreakable;

	//TODO: ARRAY OF ITENS INSIDE

	std::string generateObjectState() const;
};

#endif /* BLOCK_HPP_ */
